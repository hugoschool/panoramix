/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** panoramix.c
*/

#include "panoramix.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

static void panoramix_init(panoramix_t *panoramix, args_t *args)
{
    const size_t nb_threads = args->nb_villagers + 1;

    panoramix->threads = calloc(nb_threads, sizeof(pthread_t));
    panoramix->pot_servings = args->pot_size;
    panoramix->initial_nb_fights = args->nb_fights;
    panoramix->initial_pot_size = args->pot_size;
    panoramix->refills_left = args->nb_refills;
    panoramix->nb_villagers = args->nb_villagers;
    panoramix->villagers = villagers_init(args->nb_villagers, args->nb_fights);
    panoramix->mutex = malloc(sizeof(pthread_mutex_t));
    panoramix->druid_mutex = malloc(sizeof(pthread_mutex_t));
    panoramix->sem = calloc(1, sizeof(sem_t));
    pthread_mutex_init(panoramix->mutex, NULL);
    pthread_mutex_init(panoramix->druid_mutex, NULL);
    pthread_mutex_lock(panoramix->druid_mutex);
    sem_init(panoramix->sem, 0, panoramix->nb_villagers);
}

static void panoramix_free(panoramix_t *panoramix)
{
    free(panoramix->threads);
    villagers_free(panoramix->villagers);
    pthread_mutex_destroy(panoramix->mutex);
    pthread_mutex_destroy(panoramix->druid_mutex);
    free(panoramix->mutex);
    free(panoramix->druid_mutex);
}

void panoramix_launch(panoramix_t *panoramix)
{
    villager_thread_t *v_infos = calloc(panoramix->nb_villagers,
        sizeof(villager_thread_t));
    unsigned int i = 0;

    for (; i < panoramix->nb_villagers; i++) {
        v_infos[i].panoramix = panoramix;
        v_infos[i].i = i;
        pthread_create(&panoramix->threads[i], NULL,
            &villagers_routine, &v_infos[i]);
    }
    pthread_create(&panoramix->threads[i], NULL, &druid_routine, panoramix);
    for (unsigned int i = 0; i < panoramix->nb_villagers + 1; i++)
        pthread_join(panoramix->threads[i], NULL);
    free(v_infos);
}

void panoramix(args_t *args)
{
    panoramix_t panoramix;

    panoramix_init(&panoramix, args);
    panoramix_launch(&panoramix);
    panoramix_free(&panoramix);
}
