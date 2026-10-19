#include "codexion_header.h"

void* go_fifo(void* arg)
{
	Coder *coder;
	Data *data;
	Queue *queue;

	coder = (Coder *)arg;

	data = coder->data;
	queue = coder->data->queue;

	if (data->nb_coders == 1)
	{
		while (1)
		{
			if (!checks_simulation(coder))
				return NULL;
		}
	}

	while (coder->compile_count < data->nb_compiles)
	{
		if (!checks_simulation(coder))
			break;
    	pthread_mutex_lock(&queue->q_mutex);

		push_queue(queue, coder);

    	pthread_mutex_unlock(&queue->q_mutex);

		if (!until_i_am_top_fifo(queue, coder))
			break;
		
		if (!compiling(coder))
			break;
	}

	return NULL; 
}

void start_fifo(Coder** Coders, Data data)
{
	int i;
	Coder* coder;
	i = 0;

	while (i < data.nb_coders)
	{
		coder = Coders[i];
		pthread_create(&coder->thread, NULL, &go_fifo, coder);
		i++;
	}
	i = 0;
	while (i < data.nb_coders)
	{
		coder = Coders[i];
		pthread_join(coder->thread, NULL);
		i++;
	} 
	
}
