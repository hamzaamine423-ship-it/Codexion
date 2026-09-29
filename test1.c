#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>




void* routine(void *arg){
    int parent_id = *(int*)arg;
    if (getppid() != parent_id){
        printf("here\n");
    }
    while(1){
        sleep(1);
    }
}


int main(){
    int parent_id = getppid();  
    
    for (int i= 0; i< 1000; i++){
        int pid = fork();
        if (pid == -1){
            printf("Failed to create a child process.\n");
        }
    }

    pthread_t th[2048];

    for(int i = 0; i < 2048; i++){
        if (pthread_create(th + i, NULL, &routine, &parent_id) != 0){
            perror("Failed to create a thread");
        }
    }

    while(1){
        sleep(1);
    }
    for(int i = 0; i < 2048; i++){
        if (pthread_join(th[i], NULL) != 0){
            perror("Failed to create a thread");
        }
    }


    
}