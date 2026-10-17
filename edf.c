#include "codexion_header.h"

void* go_edf(void* arg)
{
	Coder *coder;
	Data *data;
	Heap *heap;

	coder = (Coder *)arg;

	data = coder->data;
	heap = coder->data->heap;

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
    	pthread_mutex_lock(&coder->data->global_mutex);
		if (!checks_simulation(coder))
			break;
		push_heap_edf(heap, coder);

    	pthread_mutex_unlock(&coder->data->global_mutex);
		if (!until_i_am_top(heap, coder))
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

void push_heap_edf2(Heap* heap, int child_i)
{
	Coder* parent;
	Coder* child;
    int parent_burnout;
    int child_burnout;
    int parent_i;

	while (1)
	{
		parent_i = child_i / 2;
		parent = heap->Coders[parent_i - 1];
		child = heap->Coders[child_i - 1];
        parent_burnout = parent->last_compile;
        child_burnout = child->last_compile;
		if (parent_burnout < child_burnout)
		{
			heap->Coders[parent_i - 1] = child;
			heap->Coders[child_i - 1] = parent;
			child_i = parent_i;
			if (child_i <= 1)
				break;
		}
		else
			break;
	}
}


void push_heap_edf(Heap* heap, Coder* new_coder)
{
    int child_i;

	if (!heap->size)
	{
		heap->Coders[0] = new_coder;
        heap->size++;
		return;
	}
	
	heap->Coders[heap->size] = new_coder;
	heap->size++;

	child_i = heap->size;

    push_heap_edf2(heap, child_i);
}


