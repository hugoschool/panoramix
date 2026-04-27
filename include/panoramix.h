/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** panoramix.h
*/

#ifndef PANORAMIX_H_
    #define PANORAMIX_H_

    #include <pthread.h>
    #include <semaphore.h>

    #define USAGE_OPTIONS "<nb_villagers> <pot_size> <nb_fights> <nb_refills>"
    #define USAGE "USAGE: ./panoramix "USAGE_OPTIONS

typedef struct {
    unsigned int nb_villagers;
    unsigned int pot_size;
    unsigned int nb_fights;
    unsigned int nb_refills;
} args_t;

    #define VILLAGER_I(i) thread->panoramix->villagers[i]

    #define V_PRE "Villager %ld: "
    #define VILLAGER_START V_PRE "Going into battle!\n"
    #define VILLAGER_DRINK V_PRE "I need a drink... I see %u servings left.\n"
    #define VILLAGER_POTION V_PRE "Hey Pano wake up! We need more potion.\n"
    #define VILLAGER_FIGHT V_PRE "Take that roman scum! Only %u left.\n"
    #define VILLAGER_END V_PRE "I'm going to sleep now.\n"

typedef struct {
    unsigned int nb_fights;
} villager_t;

villager_t *villagers_init(unsigned int nb_villagers, unsigned int nb_fights);
void villagers_free(villager_t *villagers);
void *villagers_routine(void *arg);

    #define D_PRE "Druid: "
    #define DRUID_START D_PRE "I'm ready... but sleepy...\n"
    #define __D_W_1 "Ah! Yes, yes, I'm awake! Working on it! "
    #define __D_W_2 "Beware I can only make %u more refills after this one.\n"
    #define DRUID_WORK D_PRE __D_W_1 __D_W_2
    #define DRUID_END D_PRE "I'm out of viscum. I'm going back to... zZz\n"

typedef struct {
    pthread_t *threads;
    int pot_servings;
    unsigned int nb_villagers;
    unsigned int initial_nb_fights;
    unsigned int initial_pot_size;
    int refills_left;
    villager_t *villagers;
    pthread_mutex_t *mutex;
    pthread_mutex_t *druid_mutex;
    sem_t *sem;
} panoramix_t;

typedef struct {
    panoramix_t *panoramix;
    size_t i;
} villager_thread_t;

void *druid_routine(void *arg);
void panoramix(args_t *args);

#endif
