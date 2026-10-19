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

    pthread_mutex_unlock(&left->d_mutex);
    pthread_mutex_unlock(&right->d_mutex);
    
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    return 1;
}


int check_last_compile(Coder *coder)
{
    long current_time;
    Data *data;
    int res;

    data = coder->data;
    pthread_mutex_lock(&coder->c_mutex);
    if (!coder->last_compile)
    {
        pthread_mutex_unlock(&coder->c_mutex);
        return 1;
    }

    
    current_time = gettime();
    current_time -= data->start_time;
    
    
    if (current_time > coder->last_compile + (data->time_to_compile + data->dongle_cooldown) * 2)
        res = 1;
    else
        res = 0;

    pthread_mutex_unlock(&coder->c_mutex);
    return res;
}

int checks_takes_dongles(Coder *coder)
{
    Dongle *left;
    Dongle *right;
    long current_time;

    left = coder->left_dongle;
    right = coder->right_dongle;

    pthread_mutex_lock(&left->d_mutex);
    pthread_mutex_lock(&right->d_mutex);

    if (left->taken || right->taken )
    {
        pthread_mutex_unlock(&left->d_mutex);
        pthread_mutex_unlock(&right->d_mutex);
        return 0;
    }

    if (left->released == -1 && right->released == -1)
        return taking_dongles(coder);

    current_time = gettime();
    current_time -= coder->data->start_time;
    if (current_time > right->released + right->coolddown)
    {
        if (current_time > (left->released + left->coolddown))
        {
            
            return taking_dongles(coder);
        }
    }
    pthread_mutex_unlock(&left->d_mutex);
    pthread_mutex_unlock(&right->d_mutex);

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