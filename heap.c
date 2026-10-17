#include "codexion_header.h"


void check_heap(Heap* heap)
{
    for (int i = 0; i < heap->size; i++)
        printf("heap[%d]: %d\n", i + 1, heap->Coders[i]->index);
}


int until_i_am_top(Heap *heap, Coder *coder)
{
    int res;
    int taken;
    taken = 0;

	while (1){
		pthread_mutex_lock(&coder->data->global_mutex);
        if (heap->size && heap->Coders[0] == coder)
		{
            pop_heap(heap);
            pthread_mutex_unlock(&coder->data->global_mutex);
            pthread_cond_broadcast(&coder->data->cond);
            break;
		}
        else
            pthread_cond_wait(&coder->data->cond, &coder->data->global_mutex);

        pthread_mutex_unlock(&coder->data->global_mutex);
    }

    pthread_mutex_lock(&coder->left_dongle->d_mutex);
    pthread_mutex_lock(&coder->right_dongle->d_mutex);

    while (1)
    {
        res = checks_cooldown(coder);
        if (res == 2)
        {
            pthread_mutex_unlock(&coder->left_dongle->d_mutex);
            pthread_mutex_unlock(&coder->right_dongle->d_mutex);
            return 0;

        }
        if (res)
            break;
    }
    pthread_mutex_unlock(&coder->left_dongle->d_mutex);
    pthread_mutex_unlock(&coder->right_dongle->d_mutex);
	return 1;
}


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

	if (!heap->size)
		return NULL;
	top_coder = heap->Coders[0];

	remove_top_heap(heap);

	return top_coder;
}

Heap* create_heap(Data data)
{
	Heap* heap;

	heap = malloc(sizeof(Heap));
	if (!heap)
		return NULL;

	heap->size = 0;
    heap->push_access = 1;

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
