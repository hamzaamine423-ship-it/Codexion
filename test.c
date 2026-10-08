#include <stdio.h>
#include <pthread.h>


int counter = 0;

pthread_mutex_t mutex_A;
pthread_mutex_t mutex_B;


void* increment(void* arg)
{
    pthread_mutex_t mutex = *(pthread_mutex_t *)arg;
    for(int i = 0; i < 10; i++)
    {
        counter++;
    }
    

}


int main()
{
    pthread_t t0;
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;

    pthread_mutex_init(&mutex_A, NULL);
    pthread_mutex_init(&mutex_B, NULL);

    for (int i = 0; )


}