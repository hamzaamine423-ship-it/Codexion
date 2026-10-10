#include "my_header.h"


void check_heap_visiters(Heap *heap)
{
	int i;

	i = 0;
	// printf("in checker of heap \n");
	while(i < heap->size){
		// printf("heap[%d]= %d\n", i + 1, heap->Coders[i]->index);
		i++;
	}
}


void taking_dongles(Coder *coder)
{
	Dongle *left_dongle;
	Dongle *right_dongle;

	left_dongle = coder->left_dongle;
	right_dongle = coder->right_dongle;

	pthread_mutex_lock(&left_dongle->mutex);
	pthread_mutex_lock(&right_dongle->mutex);

}

int i_am_the_top_heap(Heap *heap, Coder *coder)
{
	while (1){
		if (heap->Coders[0])
		{
			if(heap->Coders[0] == coder)
				break;
		}
	}
	return 1;
}


void* go(void* arg)
{
	Coder *coder;
	Data *data;
	Heap *heap;
	int i;
	coder = (Coder *)arg;

	data = coder->data;
	heap = coder->data->heap;

	i = 0;

	while (coder->compile_count < data->nb_compiles)
	{
		push_heap(heap, coder);
		// check_heap_visiters(heap);

		if (i_am_the_top_heap(heap, coder))
			pop_heap(heap);

		taking_dongles(coder);

		compiling(coder);
	}


	return NULL;
}


void start(Coder** Coders, Heap* heap, Data data)
{
	int i;
	Coder* coder;
	i = 0;

	while (i < data.nb_coders)
	{
		coder = Coders[i];
		pthread_create(&coder->thread, NULL, &go, coder);
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


void leak_test()
{
    system("leaks codexion");
}

int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Heap *heap;
	Coder *tmp;
	struct timespec ts;
	int *po;

	po = malloc(sizeof(int) * 9999);

	atexit(leak_test);

	list = parse(ac, av);
	if (!list)
		exit(0);

	data = create_data(&list, av[8]);

	Coders = creating_coders(&data);
	if (!Coders)
		exit(0);

	heap = create_heap_FIFO(data);
	if (!heap)
		free_and_exit(Coders, NULL);

	data.heap = heap;

	start(Coders, heap, data);


	free_and_exit(Coders, &heap);

	return 0;
}



