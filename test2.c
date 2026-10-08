#include <stdio.h>
#include <pthread.h>


void *routine(void *arg)
{
    int i = *(int *)arg;

    printf("Thread index %d created\n", i);
    fflush(stdout);

    while(1)
        i++;
    return NULL;
}

int main(void)
{
    pthread_t threads[100];
    int indexes[100];

    for (int i = 0; i < 100; i++)
    {
        indexes[i] = i;

        pthread_create(&threads[i], NULL, routine, &indexes[i]);
    }

    for (int i = 0; i < 100; i++)
        pthread_join(threads[i], NULL);

    return 0;
}