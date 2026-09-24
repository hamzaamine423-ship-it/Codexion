#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int	main(int argc, char *argv[])
{
	int pid = getppid();
	int arr[] = {1, 2, 5, 7, 9, 3, 1};
	int end, start;
	int fdc[2];

	if (pipe(fd) == -1){
		printf("An error occur in the pipe call!!");
		return 4;
	}

	int id = fork();

	if (id == 0){
		start = 0;
		end = (sizeof(arr) / sizeof(int)) / 2;
	}
	else{
		start = (sizeof(arr) / sizeof(int)) / 2;
		end = sizeof(arr) / sizeof(int);
	}
	int i;
	int sum = 0;
	for (i = start; i < end; i++){
		sum += arr[i];
	}

	if (id == 0){
		printf("sum from child: %d\n", sum);
	}
	else{
		printf("sum from Parent: %d\n", sum);
	}
	if (wait(NULL) == -1){
		printf("An error occur in the wait call!!");
		return 5;
	}
	


	return (0);
}
