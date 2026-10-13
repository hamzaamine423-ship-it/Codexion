#include "my_header.h"


void checks_cooldown(Coder* coder)
{
    Dongle *left_dl;
	Dongle *right_dl;


	left_dl = coder->left_dongle;
	right_dl = coder->right_dongle;
    size_t current_time;

    current_time = gettime();
    current_time -= coder->data->start_time;

    pthread_mutex_lock(&coder->data->global_mutex);

    if (current_time >= left_dl->coolddown + left_dl->released)
        left_dl->available = 1;

    if (current_time >= right_dl->coolddown + right_dl->released)
        right_dl->available = 1;
    pthread_mutex_unlock(&coder->data->global_mutex);
    
}


void taking_dongles(Coder *coder)
{
	Dongle *lf_dl;
	Dongle *rt_dl;
    size_t current_time;

	lf_dl = coder->left_dongle;
	rt_dl = coder->right_dongle;


    pthread_mutex_lock(&lf_dl->mutex);
    pthread_mutex_lock(&rt_dl->mutex);


    while (!lf_dl->available || !rt_dl->available)
        checks_cooldown(coder);


    pthread_mutex_lock(&coder->data->global_mutex);
    lf_dl->available = 0;
    rt_dl->available = 0;
    current_time = gettime();
    current_time -= coder->data->start_time;
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    pthread_mutex_unlock(&coder->data->global_mutex);

}

void unlock_dongles(Dongle* left_d, Dongle* right_d)
{
    pthread_mutex_unlock(&left_d->mutex);
    pthread_mutex_unlock(&right_d->mutex);
}
