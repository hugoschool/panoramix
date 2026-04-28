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

void *druid_routine(void *arg)
{
    panoramix_t *panoramix = arg;

    printf(DRUID_START);
    while (panoramix->refills_left > 0) {
        sem_wait(panoramix->druid_sem);
        if (panoramix->refills_left == -1) {
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
