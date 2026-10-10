#include "my_header.h"



void fix_top(Heap *heap)
{
    Coder   *cur_coder;
    Coder   *ch_left;
    Coder   *ch_right;
    int     i;

    i = 1;
    while(i < heap->size)
    {
        cur_coder = heap->Coders[i - 1];
        if ((i * 2) <= heap->size)
        {
            ch_left = heap->Coders[i * 2 - 1];
            if (ch_left->index < cur_coder->index)
            {            
                heap->Coders[i - 1] = ch_left;
                heap->Coders[i * 2 - 1] = cur_coder;
                if (i - 1 > 1)
                    i -= 1;
                else
                    i = 1;
                continue;
            }
        }
        if ((i * 2 + 1) <= heap->size)
        {
            ch_right = heap->Coders[i * 2];
            if (ch_right->index < cur_coder->index)
            {
                heap->Coders[i - 1] = ch_right;
                heap->Coders[i * 2] = cur_coder;
                if (i - 2 > 1)
                    i -= 2;
                else
                   i = 1;
                continue;
            }
        }
        i++;
    }
}

void remove_top_heap(Heap* heap)
{
    Coder *tmp_coder;
    int len;

    len = heap->size;

    if (heap->size == 0)
        return;

    if (heap->size == 1)
    {
        heap->size--;
        return;
    }
    tmp_coder = heap->Coders[0];
    heap->Coders[0] = heap-> Coders[len - 1];
    heap->Coders[len - 1] = tmp_coder;

    heap->size--;
    fix_top(heap);

}

Coder* pop_heap(Heap* heap)
{
	Coder* top_coder;

	if (heap->size == 0)
		return NULL;
	top_coder = heap->Coders[0];

	remove_top_heap(heap);

	return top_coder;
}

Heap* create_heap_FIFO(Data data)
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

	return heap;
}

void push_heap(Heap* heap, Coder* new_coder)
{
	int child_i;
	int parent_i;
	Coder* parent;
	Coder* child;

	if (!heap->size)
	{
		heap->Coders[0] = new_coder;
        heap->size++;
		return;
	}
	
	heap->Coders[heap->size] = new_coder;
	heap->size++;

	child_i = heap->size;
	while (1)
	{
		parent_i = child_i / 2;
		parent = heap->Coders[parent_i - 1];
		child = heap->Coders[child_i - 1];
		if (parent->index > child->index)
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
