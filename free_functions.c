#include "codexion_header.h"


void free_heap(Heap *heap)
{
	if (heap->mutex_initialized)
		pthread_mutex_destroy(&heap->h_mutex);

	if (heap->cond_initialized)
		pthread_cond_destroy(&heap->h_cond);

	free(heap->Coders);
	heap->Coders = NULL;
	free(heap);
	heap = NULL;
}

void free_queue(Queue *queue)
{
	if (queue->mutex_initialized)
		pthread_mutex_destroy(&queue->q_mutex);

	if (queue->cond_initialized)
		pthread_cond_destroy(&queue->q_cond);

	free(queue->Coders);
	queue->Coders = NULL;
	free(queue);
	queue = NULL;
}


void free_coders(Coder **Coders)
{
	int i;
	Data *data;
	Coder *coder;

	data = Coders[0]->data;

	i = 0;
	while(i < (*data).nb_coders){
		if (Coders[i])
		{
			coder = Coders[i];
			if(coder->left_dongle)
			{
				if (coder->left_dongle->mutex_initialized)
					pthread_mutex_destroy(&coder->left_dongle->d_mutex);
				free(coder->left_dongle);
			}
			free(coder);
		}
		else
			break;
		i++;
	}
	free(Coders);

}

void free_and_exit(Data *data)
{
	if (data->Coders)
		free_coders(data->Coders);

	if (ft_cmp(data->scheduler, "edf"))
	{
		if (data->heap)
			free_heap(data->heap);
	}
	else
	{
		if (data->queue)
			free_queue(data->queue);
	}

	if (data->mutex_initialized)
		pthread_mutex_destroy(&data->global_mutex);
	
	if (data->cond_initialized)
		pthread_cond_destroy(&data->cond);

	if (data->monitor)
		free(data->monitor);
	exit(0);
}
