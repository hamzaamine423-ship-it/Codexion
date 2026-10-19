#include "codexion_header.h"


int until_i_am_top_fifo(Queue *queue, Coder *coder)
{
    int res;

    pthread_mutex_lock(&queue->q_mutex);
	while (1)
    {
        if (!checks_simulation(coder))
        {
            pthread_mutex_unlock(&queue->q_mutex);
            return 0;
        }
        if (queue->size && queue->Coders[queue->front] == coder)
		{
            pop_queue(queue);
            pthread_mutex_unlock(&queue->q_mutex);
            pthread_cond_broadcast(&queue->q_cond);
            break;
		}
        else
            pthread_cond_wait(&queue->q_cond, &queue->q_mutex);
    }

    while (1)
    {
        if (!checks_simulation(coder))
            return 0;
        res = checks_takes_dongles(coder);


        if (res)
            break;
        usleep(100);
    }

	return 1;
}