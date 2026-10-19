#include "codexion_header.h"

int take_dongles_edf2(Coder *coder)
{
    if (coder->index == 1)
    {
        pthread_mutex_lock(&coder->left_dongle->d_mutex);
        pthread_mutex_lock(&coder->right_dongle->d_mutex);
    }
    else
    {
        pthread_mutex_lock(&coder->right_dongle->d_mutex);
        pthread_mutex_lock(&coder->left_dongle->d_mutex);
    }

    while (1)
    {
        if (!checks_simulation(coder))
        {
            pthread_mutex_unlock(&coder->right_dongle->d_mutex);
            pthread_mutex_unlock(&coder->left_dongle->d_mutex);
            return 0;
        }
        if(checks_takes_dongles(coder))
            break;

        usleep(5);
    }
    return 1;
}

int take_dongles_edf(Heap *heap, Coder *coder)
{

	while (1)
    {
        pthread_mutex_lock(&heap->h_mutex);
        if (!checks_simulation(coder))
        {
            pthread_mutex_unlock(&heap->h_mutex);
            return 0;
        }
        if (heap->size && heap->Coders[0] == coder)
		{
            pop_heap(heap);
            pthread_mutex_unlock(&heap->h_mutex);
            break;
		}
        else
        {
            pthread_mutex_unlock(&heap->h_mutex);
            usleep(10);
        }
    }
    return take_dongles_edf2(coder);
}

int fix_left(Heap *heap, int i, Coder *cur_coder)
{
    Coder *ch_left;

    if (i * 2 > heap->size)
        return (0);
    ch_left = heap->Coders[i * 2 - 1];
    if (ch_left->last_compile < cur_coder->last_compile)
    {
        heap->Coders[i - 1] = ch_left;
        heap->Coders[i * 2 - 1] = cur_coder;
        return (1);
    }
    return (0);
}

int fix_right(Heap *heap, int i, Coder *cur_coder)
{
    Coder *ch_right;

    if (i * 2 + 1 > heap->size)
        return (0);
    ch_right = heap->Coders[i * 2];
    if (ch_right->last_compile < cur_coder->last_compile)
    {
        heap->Coders[i - 1] = ch_right;
        heap->Coders[i * 2] = cur_coder;
        return (1);
    }
    return (0);
}

void fix_heap(Heap *heap)
{
    Coder *cur_coder;
    int   i;

    i = 1;
    while (i < heap->size)
    {
        cur_coder = heap->Coders[i - 1];
        if (fix_left(heap, i, cur_coder))
        {
            if (i - 1 > 1)
                i -= 1;
            else
                i = 1;
            continue;
        }
        if (fix_right(heap, i, cur_coder))
        {
            if (i - 2 > 1)
                i -= 2;
            else
                i = 1;
            continue;
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
