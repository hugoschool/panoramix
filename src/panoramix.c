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
    panoramix->nb_villagers = args->nb_villagers;
    panoramix->villagers = villagers_init(args->nb_villagers, args->nb_fights);
    pthread_mutex_init(panoramix->mutex, NULL);
}

static void panoramix_free(panoramix_t *panoramix)
{
    free(panoramix->threads);
    villagers_free(panoramix->villagers);
    pthread_mutex_destroy(panoramix->mutex);
}

void panoramix_launch(panoramix_t *panoramix)
{
    sem_t *sem = calloc(1, sizeof(sem_t));
    villager_thread_t *v_infos = calloc(panoramix->nb_villagers,
        sizeof(villager_thread_t));

    sem_init(sem, 0, panoramix->nb_villagers);
    for (unsigned int i = 0; i < panoramix->nb_villagers; i++) {
        v_infos[i].panoramix = panoramix;
        v_infos[i].i = i;
        v_infos[i].sem = sem;
        pthread_create(&panoramix->threads[i], NULL,
            &villagers_routine, &v_infos[i]);
    }
    for (unsigned int i = 0; i < panoramix->nb_villagers; i++)
        pthread_join(panoramix->threads[i], NULL);
    free(v_infos);
    sem_destroy(sem);
}

void panoramix(args_t *args)
{
    panoramix_t panoramix;

    panoramix_init(&panoramix, args);
    panoramix_launch(&panoramix);
    panoramix_free(&panoramix);
}
