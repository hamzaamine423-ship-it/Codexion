#include "codexion_header.h"

void taking_dongles(Coder* coder)
{
    Dongle* left;
    Dongle* right;
    long current_time;

    left = coder->left_dongle;
    right = coder->right_dongle;
    current_time = gettime() - coder->data->start_time;


    left->taken = 1;
    right->taken = 1;
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
}

int checks_cooldown(Coder *coder)
{
    Dongle *left;
    Dongle *right;
    long current_time;

    left = coder->left_dongle;
    right = coder->right_dongle;

    if (pthread_mutex_lock(&coder->data->global_mutex) != 0)
        exit(1);
    if (left->taken || right->taken )
    {
        if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
            exit(1);
        return 0;
    }

    if (left->released == -1 && right->released == -1)
    {
        taking_dongles(coder);
        if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
            exit(1);
        return 1;
    }

    current_time = gettime();
    current_time -= coder->data->start_time;
    // printf("current time: %d\n", current_time);
    // printf("right released: %d\n", right->released);
    // printf("right cooldown: %d\n", right->coolddown);
    // printf("left released: %d\n", left->released);
    // printf("left cooldown: %d\n", left->coolddown);
    // printf("the sum left: %d\n", left->released + left->coolddown);
    if (current_time > right->released + right->coolddown)
    {

        if (current_time > (left->released + left->coolddown))
        {

            taking_dongles(coder);
            if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
                exit(1);
            return 1;
        }
    }

    if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
        exit(1);
    
    return 0;
}

void recovery_dongles(Coder* coder)
{
    Dongle* left;
    Dongle* right;

    left = coder->left_dongle;
    right = coder->right_dongle;

    left->taken = 0;
    right->taken = 0;
}