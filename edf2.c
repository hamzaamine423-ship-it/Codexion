#include "codexion_header.h"

int until_i_am_top_edf(Heap *heap, Coder *coder)
{
    int res;

    res = 0;
    pthread_mutex_lock(&heap->h_mutex);
	while (1)
    {
        if (!checks_simulation(coder))
        {
            pthread_mutex_unlock(&heap->h_mutex);
            return 0;
        }
        if (heap->size && heap->Coders[0] == coder)
		{
            pop_heap(heap);
            pthread_mutex_unlock(&heap->h_mutex);
            pthread_cond_broadcast(&heap->h_cond);
            break;
		}
        else
            pthread_cond_wait(&heap->h_cond, &heap->h_mutex);
    }

    while (1)
    {
        if (!checks_simulation(coder))
            return 0;
        
        res = checks_takes_dongles(coder);

        if (res)
            break;
        usleep(5);
    }

	return 1;
}


void fix_heap(Heap *heap)
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
            if (ch_left->last_compile < cur_coder->last_compile)
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
            if (ch_right->last_compile < cur_coder->last_compile)
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
    int len;

    len = heap->size;

    if (heap->size == 1)
    {
        heap->size--;
        return;
    }
    heap->Coders[0] = heap-> Coders[len - 1];

    heap->size--;
    fix_heap(heap);

}

Coder* pop_heap(Heap* heap)
{
	Coder* top_coder;

	if (!heap->size)
		return NULL;
	top_coder = heap->Coders[0];

	remove_top_heap(heap);

	return top_coder;
}