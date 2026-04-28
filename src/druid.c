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

static bool any_fights_left(panoramix_t *panoramix)
{
    for (unsigned int i = 0; i < panoramix->nb_villagers; i++) {
        pthread_mutex_lock(panoramix->villagers[i].fight_mutex);
        if (panoramix->villagers[i].nb_fights > 0) {
            pthread_mutex_unlock(panoramix->villagers[i].fight_mutex);
            return true;
        }
        pthread_mutex_unlock(panoramix->villagers[i].fight_mutex);
    }
    return false;
}

void *druid_routine(void *arg)
{
    panoramix_t *panoramix = arg;

    printf(DRUID_START);
    while (panoramix->refills_left > 0) {
        sem_wait(panoramix->druid_sem);
        if (any_fights_left(panoramix) == false) {
            break;
        }
        panoramix->pot_servings = panoramix->initial_pot_size;
        panoramix->refills_left--;
        printf(DRUID_WORK, panoramix->refills_left);
        sem_post(panoramix->villagers_sem);
    }
    printf(DRUID_END);
    return NULL;
}
