#include "my_header.h"

void push_heap_fifo2(Heap* heap, int child_i)
{
    int parent_i;
	Coder* parent;
	Coder* child;

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

void push_heap_fifo(Heap* heap, Coder* new_coder)
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

    push_heap_fifo2(heap, child_i);

}


