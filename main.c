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

int until_i_am_top(Heap *heap, Coder *coder)
{
	while (1){
		if (heap->Coders[0])
		{
			if(heap->Coders[0] == coder)
			{
				pop_heap(heap);
				break;
			}
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

		until_i_am_top(heap, coder);

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


int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Heap *heap;
	Coder *tmp;

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

	data.start_time = gettime();
	start(Coders, heap, data);

	free_and_exit(Coders, &heap);

	return 0;
}



