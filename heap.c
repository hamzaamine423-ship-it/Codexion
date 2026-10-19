#include "codexion_header.h"


void check_heap(Heap heap)
{
    for (int i = 0; i < heap.size; i++)
    {
        printf("heap[%d]: %d\n", i + 1, heap.Coders[i]->index);
		fflush(stdout);
    }
}

Heap* init_heap(Data data)
{
	Heap* heap;

	heap = malloc(sizeof(Heap));
	if (!heap)
		return NULL;

	heap->size = 0;
    heap->Coders = malloc(sizeof(Coder *) * data.nb_coders);
    if (!heap->Coders)
    {
        free(heap);
        return NULL;
    }
	heap->cond_initialized = pthread_cond_init(&heap->h_cond, NULL);
	heap->mutex_initialized = pthread_mutex_init(&heap->h_mutex, NULL);

    if (heap->mutex_initialized != 0 || heap->cond_initialized != 0)
    {
        free(heap);
        return NULL;
    }

	return heap;
}

void push_heap2(Heap* heap, int child_i)
{
	Coder* parent;
	Coder* child;
    int parent_i;

	while (1)
	{
		parent_i = child_i / 2;
		parent = heap->Coders[parent_i - 1];
		child = heap->Coders[child_i - 1];

		if (parent->last_compile > child->last_compile)
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


void push_heap(Heap* heap, Coder* new_coder)
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

    push_heap2(heap, child_i);
}

