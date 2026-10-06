#include "my_header.h"

void* routine(){
	int i = 0;

	while(1)
		i++;
	
}


int main(){

	pthread_t threads[3000];
	for (int i = 0; i < 3000; i++){
		printf("%d\n", i);
		fflush(stdout);
		if(pthread_create(&threads[i], NULL, &routine, NULL)){
			perror("Failed to create the thread!\n");
			return;
		}	
	}

	for(int i = 0; i < 3000; i++){
		if(pthread_join(threads[i], NULL)){
			perror("Failed to join the thread!\n");
			return;
		}
	}
}