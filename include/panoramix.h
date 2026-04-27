/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** panoramix.h
*/

#ifndef PANORAMIX_H_
    #define PANORAMIX_H_

    #define USAGE_OPTIONS "<nb_villagers> <pot_size> <nb_fights> <nb_refills>"
    #define USAGE "USAGE: ./panoramix "USAGE_OPTIONS

typedef struct {
    unsigned int nb_villagers;
    unsigned int pot_size;
    unsigned int nb_fights;
    unsigned int nb_refills;
} args_t;

#endif
