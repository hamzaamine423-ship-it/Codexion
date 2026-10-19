#include "codexion_header.h"

void check_queue(Queue queue)
{
    for (int i = 0; i < queue.size; i++)
    {
        int pos = (queue.front + i) % queue.capacity;

        printf("Queue[%d] = %d\n",
            i, queue.Coders[pos]->index);
    }
}


Queue* init_queue(Data data)
{
    Queue *queue;

    queue = malloc(sizeof(Queue));
    if (!queue)
        return NULL;
    
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = data.nb_coders;

    queue->Coders = malloc(sizeof(Coder *) * data.nb_coders);
    if (!queue->Coders)
    {
        free(queue);
        return NULL;
    }
    pthread_mutex_init(&queue->q_mutex, NULL);

    pthread_cond_init(&queue->q_cond, NULL);

    return queue;
}

Coder* pop_queue(Queue *queue)
{
    Coder* coder;

    if (!queue->size)
        return NULL;
    
    coder = queue->Coders[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return coder;
}

void push_queue(Queue *queue, Coder *new_coder)
{
    if (queue->size == queue->capacity)
        return;
    queue->Coders[queue->rear] = new_coder;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
}