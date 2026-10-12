#include "my_header.h"


void taking_dongles(Coder *coder)
{
	Dongle *lf_dl;
	Dongle *rt_dl;

	lf_dl = coder->left_dongle;
	rt_dl = coder->right_dongle;

    while (!lf_dl->available && !rt_dl->available)
        unlock_dongles(lf_dl, rt_dl);

    pthread_cond_wait(&lf_dl->cond, &lf_dl->mutex);
    pthread_cond_wait(&rt_dl->cond, &rt_dl->mutex);

    pthread_mutex_lock(&lf_dl->mutex);
    pthread_mutex_lock(&rt_dl->mutex);
    lf_dl->available = 0;
    rt_dl->available = 0;

}

void unlock_dongles(Dongle* left_d, Dongle* right_d)
{
    size_t current_time;

    current_time = gettime();



    if (current_time >= left_d->coolddown + left_d->released)
    {
        pthread_mutex_unlock(&left_d->mutex);
        left_d->available = 1;
        pthread_cond_signal(&left_d->cond);
    }

    if (current_time >= right_d->coolddown + right_d->released)
    {
        pthread_mutex_unlock(&right_d->mutex);
        right_d->available = 1;
        pthread_cond_signal(&right_d);
    }
}
