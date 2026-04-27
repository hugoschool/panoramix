/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** panoramix.c
*/

#include "panoramix.h"
#include <stdlib.h>

static void panoramix_init(panoramix_t *panoramix, args_t *args)
{
    const size_t nb_threads = args->nb_villagers + 1;

    panoramix->threads = calloc(nb_threads, sizeof(pthread_t));
    panoramix->pot_servings = args->pot_size;
    panoramix->nb_villagers = args->nb_villagers;
    panoramix->villagers = villagers_init(args->nb_villagers, args->nb_fights);
}

static void panoramix_free(panoramix_t *panoramix)
{
    free(panoramix->threads);
    villagers_free(panoramix->villagers);
}

void panoramix_launch(panoramix_t *panoramix)
{
    (void)panoramix;
}

void panoramix(args_t *args)
{
    panoramix_t panoramix;

    panoramix_init(&panoramix, args);
    panoramix_launch(&panoramix);
    panoramix_free(&panoramix);
}
