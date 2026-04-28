/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** druid.c
*/

#include "panoramix.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void reset_villagers(panoramix_t *panoramix)
{
    for (unsigned int i = 0; i < panoramix->nb_villagers; i++) {
        if (sem_post(panoramix->villagers_sem) != 0)
            break;
    }
}

void *druid_routine(void *arg)
{
    panoramix_t *panoramix = arg;

    printf(DRUID_START);
    while (panoramix->refills_left > 0) {
        sem_wait(panoramix->druid_sem);
        if (panoramix->pot_servings > 0) {
            sem_post(panoramix->druid_sem);
            continue;
        }
        pthread_mutex_lock(panoramix->servings_mutex);
        panoramix->pot_servings = panoramix->initial_pot_size;
        pthread_mutex_unlock(panoramix->servings_mutex);
        pthread_mutex_lock(panoramix->refills_mutex);
        panoramix->refills_left--;
        pthread_mutex_unlock(panoramix->refills_mutex);
        printf(DRUID_WORK, panoramix->refills_left);
        reset_villagers(panoramix);
    }
    printf(DRUID_END);
    return NULL;
}
