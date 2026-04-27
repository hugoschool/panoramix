/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** panoramix.h
*/

#ifndef PANORAMIX_H_
    #define PANORAMIX_H_

    #include <pthread.h>

    #define USAGE_OPTIONS "<nb_villagers> <pot_size> <nb_fights> <nb_refills>"
    #define USAGE "USAGE: ./panoramix "USAGE_OPTIONS

typedef struct {
    unsigned int nb_villagers;
    unsigned int pot_size;
    unsigned int nb_fights;
    unsigned int nb_refills;
} args_t;

    #define VILLAGER_PREFIX "Villager %ld: "
    #define VILLAGER_START VILLAGER_PREFIX "Going into battle!\n"
    #define VILLAGER_END VILLAGER_PREFIX "I'm going to sleep now.\n"

typedef struct {
    unsigned int nb_fights;
} villager_t;

villager_t *villagers_init(unsigned int nb_villagers, unsigned int nb_fights);
void villagers_free(villager_t *villagers);
void *villagers_routine(void *arg);

typedef struct {
    pthread_t *threads;
    unsigned int pot_servings;
    unsigned int nb_villagers;
    villager_t *villagers;
} panoramix_t;

typedef struct {
    panoramix_t *panoramix;
    size_t i;
} villager_thread_t;

void panoramix(args_t *args);

#endif
