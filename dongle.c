#include "codexion_header.h"


int taking_dongles(Coder* coder)
{
    Dongle* left;
    Dongle* right;
    long current_time;

    left = coder->left_dongle;
    right = coder->right_dongle;
    current_time = gettime();
    current_time -= coder->data->start_time;


    left->taken = 1;
    right->taken = 1;

    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    return 1;
}


int checks_takes_dongles(Coder *coder)
{
    Dongle *left;
    Dongle *right;
    long current_time;

    left = coder->left_dongle;
    right = coder->right_dongle;

    if (left->released == -1 && right->released == -1)
        return taking_dongles(coder);

    current_time = gettime();
    current_time -= coder->data->start_time;
    if (current_time > right->released + right->coolddown)
    {
        if (current_time > (left->released + left->coolddown))
            return taking_dongles(coder);
    }

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
    pthread_mutex_unlock(&left->d_mutex);
    pthread_mutex_unlock(&right->d_mutex);
}
