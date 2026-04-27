/*
** EPITECH PROJECT, 2026
** panoramix
** File description:
** main.c
*/

#include "panoramix.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool parse_args(args_t *args, char **argv)
{
    args->nb_villagers = atoi(argv[1]);
    args->pot_size = atoi(argv[2]);
    args->nb_fights = atoi(argv[3]);
    args->nb_refills = atoi(argv[4]);
    return args->nb_villagers > 0 && args->pot_size > 0 &&
        args->nb_fights > 0 && args->nb_refills > 0;
}

int main(int argc, char **argv)
{
    args_t args = {0};

    if (argc != 5) {
        fprintf(stderr, USAGE"\n");
        return 84;
    }
    if (parse_args(&args, argv) == false) {
        fprintf(stderr, USAGE"\n");
        fprintf(stderr, "Values must be >0.\n");
        return 84;
    }
    panoramix(&args);
    return 0;
}
