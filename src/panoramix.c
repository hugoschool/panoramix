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
    panoramix->pot_servings = args->pot_size;
    panoramix->initial_nb_fights = args->nb_fights;
    panoramix->initial_pot_size = args->pot_size;
    panoramix->refills_left = args->nb_refills;
    panoramix->nb_villagers = args->nb_villagers;
    panoramix->villagers = villagers_init(args->nb_villagers, args->nb_fights);
    panoramix->servings_mutex = malloc(sizeof(pthread_mutex_t));
    panoramix->refills_mutex = malloc(sizeof(pthread_mutex_t));
    panoramix->villagers_sem = calloc(1, sizeof(sem_t));
    panoramix->druid_sem = calloc(1, sizeof(sem_t));
    pthread_mutex_init(panoramix->servings_mutex, NULL);
    pthread_mutex_init(panoramix->refills_mutex, NULL);
    sem_init(panoramix->druid_sem, 0, 0);
    sem_init(panoramix->villagers_sem, 0, 0);
}

static void panoramix_free(panoramix_t *panoramix)
{
    villagers_free(panoramix->villagers, panoramix->nb_villagers);
    pthread_mutex_destroy(panoramix->servings_mutex);
    pthread_mutex_destroy(panoramix->refills_mutex);
    free(panoramix->servings_mutex);
    free(panoramix->refills_mutex);
    sem_destroy(panoramix->villagers_sem);
    sem_destroy(panoramix->druid_sem);
    free(panoramix->villagers_sem);
    free(panoramix->druid_sem);
}

static void panoramix_joiner(panoramix_t *panoramix,
    pthread_t *villager_threads, pthread_t druid_thread)
{
    for (unsigned int i = 0; i < panoramix->nb_villagers; i++) {
        pthread_join(villager_threads[i], NULL);
        pthread_mutex_lock(panoramix->villagers[i].fight_mutex);
        panoramix->villagers[i].nb_fights = -1;
        pthread_mutex_unlock(panoramix->villagers[i].fight_mutex);
    }
    sem_post(panoramix->druid_sem);
    pthread_join(druid_thread, NULL);
}

void panoramix_launch(panoramix_t *panoramix)
{
    villager_thread_t *v_infos = calloc(panoramix->nb_villagers,
        sizeof(villager_thread_t));
    pthread_t druid_thread;
    pthread_t villager_threads[panoramix->nb_villagers];

    pthread_create(&druid_thread, NULL, &druid_routine, panoramix);
    for (unsigned int i = 0; i < panoramix->nb_villagers; i++) {
        v_infos[i].panoramix = panoramix;
        v_infos[i].i = i;
        pthread_create(&villager_threads[i], NULL,
            &villagers_routine, &v_infos[i]);
    }
    panoramix_joiner(panoramix, villager_threads, druid_thread);
    free(v_infos);
}

void panoramix(args_t *args)
{
    panoramix_t panoramix;

    panoramix_init(&panoramix, args);
    panoramix_launch(&panoramix);
    panoramix_free(&panoramix);
}
