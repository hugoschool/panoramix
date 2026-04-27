/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** villagers.c
*/

#include "panoramix.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

villager_t *villagers_init(unsigned int nb_villagers, unsigned int nb_fights)
{
    villager_t *villagers = calloc(nb_villagers, sizeof(villager_t));

    for (unsigned int i = 0; i < nb_villagers; i++) {
        villagers[i].nb_fights = nb_fights;
    }
    return villagers;
}

void villagers_free(villager_t *villagers)
{
    free(villagers);
}

static enum loop_status villager_iteration(villager_thread_t *thread)
{
    pthread_mutex_lock(thread->panoramix->mutex);
    if (thread->panoramix->pot_servings >= 0)
        printf(VILLAGER_DRINK, thread->i, thread->panoramix->pot_servings);
    if (thread->panoramix->pot_servings == 0) {
        printf(VILLAGER_POTION, thread->i);
        thread->panoramix->pot_servings = -1;
        pthread_mutex_unlock(thread->panoramix->mutex);
        sem_post(thread->panoramix->druid_sem);
        return DONT_RELEASE;
    } else if (thread->panoramix->pot_servings < 0) {
        pthread_mutex_unlock(thread->panoramix->mutex);
        return RELEASE;
    }
    thread->panoramix->pot_servings--;
    pthread_mutex_unlock(thread->panoramix->mutex);
    VILLAGER_I(thread->i).nb_fights--;
    printf(VILLAGER_FIGHT, thread->i, VILLAGER_I(thread->i).nb_fights);
    return RELEASE;
}

void *villagers_routine(void *arg)
{
    villager_thread_t *thread = arg;

    printf(VILLAGER_START, thread->i);
    while (VILLAGER_I(thread->i).nb_fights > 0
        && thread->panoramix->refills_left >= 0) {
        sem_wait(thread->panoramix->sem);
        if (thread->panoramix->pot_servings == -1) {
            sem_post(thread->panoramix->sem);
            continue;
        }
        switch (villager_iteration(thread)) {
            case DONT_RELEASE:
                continue;
            case RELEASE:
                break;
        }
        sem_post(thread->panoramix->sem);
    }
    printf(VILLAGER_END, thread->i);
    return NULL;
}
