#include "codexion_header.h"

int take_dongles_fifo2(Coder *coder)
{
    if (coder->index == 1)
    {
        pthread_mutex_lock(&coder->left_dongle->d_mutex);
        pthread_mutex_lock(&coder->right_dongle->d_mutex);
    }
    else
    {
        pthread_mutex_lock(&coder->right_dongle->d_mutex);
        pthread_mutex_lock(&coder->left_dongle->d_mutex);
    }

    while (1)
    {
        if (!checks_simulation(coder))
        {
            pthread_mutex_unlock(&coder->right_dongle->d_mutex);
            pthread_mutex_unlock(&coder->left_dongle->d_mutex);
            return 0;
        }
        if(checks_takes_dongles(coder))
            break;

        usleep(5);
    }
    return 1;
}

int take_dongles_fifo(Queue *queue, Coder *coder)
{
	while (1)
    {
        pthread_mutex_lock(&queue->q_mutex);
        if (!checks_simulation(coder))
        {
            pthread_mutex_unlock(&queue->q_mutex);
            return 0;
        }
        if (queue->size && queue->Coders[queue->front] == coder)
		{
            pop_queue(queue);
            pthread_mutex_unlock(&queue->q_mutex);
            break;
		}
        else
        {
            pthread_mutex_unlock(&queue->q_mutex);
            usleep(10);
        }
    }
    return take_dongles_fifo2(coder);
}
