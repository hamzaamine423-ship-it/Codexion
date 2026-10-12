#include "my_header.h"


void checks_cooldown(Dongle* left_d, Dongle* right_d)
{
    size_t current_time;

    current_time = gettime();

    if (current_time >= left_d->coolddown + left_d->released)
        left_d->available = 1;

    if (current_time >= right_d->coolddown + right_d->released)
        right_d->available = 1;
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
        checks_cooldown(lf_dl, rt_dl);

    lf_dl->available = 0;
    rt_dl->available = 0;
    current_time = gettime();
    current_time -= coder->data->start_time;
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");
    put_str_codexion(coder->index, current_time, "has taken a dongle\n");

}

void unlock_dongles(Dongle* left_d, Dongle* right_d)
{
    pthread_mutex_unlock(&left_d->mutex);
    pthread_mutex_unlock(&right_d->mutex);
}
