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

typedef struct {
    unsigned int nb_fights;
} villager_t;

villager_t *villagers_init(unsigned int nb_villagers, unsigned int nb_fights);
void villagers_free(villager_t *villagers);

typedef struct {
    pthread_t *threads;
    unsigned int pot_servings;
    unsigned int nb_villagers;
    villager_t *villagers;
} panoramix_t;

void panoramix(args_t *args);

#endif
