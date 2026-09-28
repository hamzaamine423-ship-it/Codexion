#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/_pthread/_pthread_cond_t.h>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <unistd.h>

typedef struct{
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} ThreadData;

int fuel = 0;

void* Fullfuel(void* arg){
    ThreadData *data = (ThreadData *)arg;

    for (int i = 0; i<5; i++){
        pthread_mutex_lock(data->mutex);
        fuel += 60;
        printf("Filling fuel: %d\n", fuel);
        pthread_cond_broadcast(data->cond); 
        pthread_mutex_unlock(data->mutex);
        sleep(1);
    }
}

void* car(void* arg){
    ThreadData *data= (ThreadData *)arg;
    pthread_mutex_lock(data->mutex);
    while(fuel < 40){   
        printf("NO fuel. Waiting...\n");
        pthread_cond_wait(data->cond, data->mutex);
    }
    fuel -= 40;
    printf("Got fuel. now %d\n", fuel);
    pthread_mutex_unlock(data->mutex);
}


int main(){
    pthread_t th[5];
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    ThreadData data = {
        .mutex = &mutex,
        .cond = &cond
    };


    for (int i = 0; i < 5; i++){
        if (i == 4){
            if (pthread_create(th + i, NULL, &Fullfuel, &data) != 0){
                perror("Failde to create a thread!");
            }
        }
        else{
            if (pthread_create(th + i, NULL, &car, &data) != 0){
                perror("Failed to create a thread!!");
            }
        }
    }
    
    for (int i = 0; i < 5; i++){
        if(pthread_join(th[i], NULL) != 0){
            perror("Failed to join a thread!");
        }
    }

}