#include "my_header.h"


void free_and_exit(Coder** Coders, Data data, int* list)
{
	if (Coders)
		free_coders(Coders, data);

	free(list);

	exit(0);
}



void free_coders(Coder **Coders, Data data)
{
	int i;
	Coder *coder;

	i = 0;
	while(i < data.nb_coders){
		coder = Coders[i];
		if(coder->right_dongle)
			free(coder->right_dongle);
		if (coder)
			free(coder);
		i++;
	}

	free(Coders);
}
