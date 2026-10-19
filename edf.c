#include "codexion_header.h"

void* go_edf(void* arg)
{
	Coder *coder;
	Data *data;
	Heap *heap;

	coder = (Coder *)arg;
	data = coder->data;
	heap = coder->data->heap;
	while (coder->compile_count < data->nb_compiles)
	{
		if (!checks_simulation(coder))
			break;
    	pthread_mutex_lock(&heap->h_mutex);

		push_heap(heap, coder);

    	pthread_mutex_unlock(&heap->h_mutex);

		if (!take_dongles_edf(heap, coder))
			break;

		if (!compiling(coder))
			break;
	}

	return NULL;
}

void start_edf(Coder** Coders, Data data)
{
	int i;
	Coder* coder;

	i = 0;
	if (Coders[0]->data->nb_coders == 1)
	{
		while (1)
		{
			if (!checks_simulation(Coders[0]))
				return;
		}
	}
	while (i < data.nb_coders)
	{
		coder = Coders[i];
		pthread_create(&coder->thread, NULL, &go_edf, coder);
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
