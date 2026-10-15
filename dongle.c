#include "codexion_header.h"

void taking_dongles(Coder* coder)
{
    Dongle* left;
    Dongle* right;
    size_t current_time;

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
    size_t current_time;

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
    if (current_time > right->released + right->coolddown)
    {
        if (current_time > left->released + left->coolddown)
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