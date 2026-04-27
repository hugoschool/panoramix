/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** villagers.c
*/

#include "panoramix.h"
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

void *villagers_routine(void *arg)
{
    villager_thread_t *thread = arg;

    printf(VILLAGER_START, thread->i);
    printf(VILLAGER_END, thread->i);
    return NULL;
}
