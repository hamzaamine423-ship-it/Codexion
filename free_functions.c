#include "my_header.h"


void free_and_exit(Dongle **Dongles,Coder** Coders, Data data, int* list)
{
	if (Dongles)
		free_dongles(Dongles, data);

	if (Coders)
		free_coders(Coders, data);

	free(list);

	exit(0);
}


void free_dongles(Dongle **Dongles, Data data)
{
	int i;

	i = 0;
	while(i < data.nb_coders){

		pthread_mutex_destroy(&Dongles[i]->mutex);

		free(Dongles[i]);
		i++;
	}

	free(Dongles);
}


void free_coders(Coder **Coders, Data data)
{
	int i;

	i = 0;
	while(i < data.nb_coders){
		free(Coders[i]);
		i++;
	}

	free(Coders);
}
