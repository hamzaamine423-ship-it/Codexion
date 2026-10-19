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
	pthread_cond_init(&heap->h_cond, NULL);

	pthread_mutex_init(&heap->h_mutex, NULL);

	return heap;
}
