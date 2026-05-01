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
        villagers[i].fight_mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(villagers[i].fight_mutex, NULL);
    }
    return villagers;
}

void villagers_free(villager_t *villagers, unsigned int nb_villagers)
{
    for (unsigned int i = 0; i < nb_villagers; i++) {
        pthread_mutex_destroy(villagers[i].fight_mutex);
        free(villagers[i].fight_mutex);
    }
    free(villagers);
}

static void villager_iteration(villager_thread_t *thread)
{
    if (thread->panoramix->pot_servings >= 0)
        printf(VILLAGER_DRINK, thread->i, thread->panoramix->pot_servings);
    if (thread->panoramix->pot_servings == 0) {
        printf(VILLAGER_POTION, thread->i);
        thread->panoramix->pot_servings = -1;
        sem_post(thread->panoramix->druid_sem);
        sem_wait(thread->panoramix->villagers_sem);
    } else if (thread->panoramix->pot_servings < 0) {
        sem_wait(thread->panoramix->villagers_sem);
    }
    thread->panoramix->pot_servings--;
    pthread_mutex_lock(VILLAGER_I(thread->i).fight_mutex);
    VILLAGER_I(thread->i).nb_fights--;
    pthread_mutex_unlock(VILLAGER_I(thread->i).fight_mutex);
    printf(VILLAGER_FIGHT, thread->i, VILLAGER_I(thread->i).nb_fights);
}

void *villagers_routine(void *arg)
{
    villager_thread_t *thread = arg;

    printf(VILLAGER_START, thread->i);
    while (VILLAGER_I(thread->i).nb_fights > 0) {
        pthread_mutex_lock(thread->panoramix->servings_mutex);
        if (thread->panoramix->pot_servings == -1)
            continue;
        villager_iteration(thread);
        pthread_mutex_unlock(thread->panoramix->servings_mutex);
    }
    printf(VILLAGER_END, thread->i);
    return NULL;
}
