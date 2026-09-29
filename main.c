#include "my_header.h"
#include <pthread.h>

int main(int ac, char* av[]){
	int *list;

	list = parse(ac, av);

	for(int i= 0; i < 7; i++){
		printf("list[%d]: %d\n", i, list[i]);
	}
	printf("Succes!");


	free(list);
	return 0;
}
