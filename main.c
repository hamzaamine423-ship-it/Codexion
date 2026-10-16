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
    	if (pthread_mutex_lock(&coder->data->global_mutex) != 0)
    	    exit(1);
		push_heap_edf(heap, coder);
    	if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
    	    exit(1);

		until_i_am_top(heap, coder);

		compiling(coder);
	}

 
	return NULL;
}

void start_edf(Coder** Coders, Data data)
{
	int i;
	Coder* coder;
	i = 0;

	while (i < data.nb_coders)
	{
		coder = Coders[i];
		if (pthread_create(&coder->thread, NULL, &go_edf, coder) != 0)
			exit(1);
		i++;
	}
	i = 0;
	while (i < data.nb_coders)
	{
		coder = Coders[i];
		if (pthread_join(coder->thread, NULL) != 0)
			exit(1);
		i++;
	}
}

void* go_fifo(void* arg)
{
	Coder *coder;
	Data *data;
	Heap *heap;

	coder = (Coder *)arg;

	data = coder->data;
	heap = coder->data->heap;


	while (coder->compile_count < data->nb_compiles)
	{
    	if (pthread_mutex_lock(&coder->data->global_mutex) != 0)
    	    exit(1);
		push_heap_fifo(heap, coder);
    	if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
    	    exit(1);

		until_i_am_top(heap, coder);
		
		compiling(coder);
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
		if (pthread_create(&coder->thread, NULL, &go_fifo, coder) != 0)
			exit(1);
		i++;
	}
	i = 0;
	while (i < data.nb_coders)
	{
		coder = Coders[i];
		if (pthread_join(coder->thread, NULL) != 0)
			exit(1);
		i++;
	} 
	
}

int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Heap *heap;
	// Monitor *monitor;

	list = parse(ac, av);
	if (!list)
		exit(0);

	data = create_data(&list, av[8]);

	Coders = creating_coders(&data);
	if (!Coders)
		exit(0);

	data.Coders = Coders;

	heap = create_heap(data);
	if (!heap)
		free_and_exit(Coders, NULL);

	data.heap = heap;

	// monitor = start_monitor(Coders, data.time_to_burnout);
	data.start_time = gettime();


	if (ft_cmp(data.scheduler, "edf"))
		start_edf(Coders, data);
	else
		start_fifo(Coders, data);
		
	printf("\nfinished sucess!\n");
	// pthread_join(monitor->thread, NULL);

	free_and_exit(Coders, &heap);

	return 0;
}



