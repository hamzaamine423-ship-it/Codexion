#include "codexion_header.h"

int	take_dongles_fifo2(t_coder *cooder)
{
	if (cooder->index == 1)
	{
		pthread_mutex_lock(&cooder->left_dongle->d_mutex);
		pthread_mutex_lock(&cooder->right_dongle->d_mutex);
	}
	else
	{
		pthread_mutex_lock(&cooder->right_dongle->d_mutex);
		pthread_mutex_lock(&cooder->left_dongle->d_mutex);
	}
	while (1)
	{
		if (!checks_simulation(cooder))
		{
			pthread_mutex_unlock(&cooder->right_dongle->d_mutex);
			pthread_mutex_unlock(&cooder->left_dongle->d_mutex);
			return (0);
		}
		if (checks_takes_dongles(cooder))
			break ;
		usleep(5);
	}
	return (1);
}

int	take_dongles_fifo(t_queue *queuee, t_coder *cooder)
{
	while (1)
	{
		pthread_mutex_lock(&queuee->q_mutex);
		if (!checks_simulation(cooder))
		{
			pthread_mutex_unlock(&queuee->q_mutex);
			return (0);
		}
		if (queuee->size && queuee->coders[queuee->front] == cooder)
		{
			pop_queue(queuee);
			pthread_mutex_unlock(&queuee->q_mutex);
			break ;
		}
		else
		{
			pthread_mutex_unlock(&queuee->q_mutex);
			usleep(10);
		}
	}
	return (take_dongles_fifo2(cooder));
}
