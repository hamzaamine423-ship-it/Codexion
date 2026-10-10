#include "my_header.h"

void free_and_exit(Coder** Coders, Heap** heap)
{
	if (Coders)
	{
		free_coders(Coders);
	}

	if (*heap)
	{
		free((*heap)->Coders);
		free(*heap);
		*heap = NULL;
	}
	exit(0);
}


void free_coders(Coder **Coders)
{
	int i;
	Data *data;
	Coder *coder;

	data = Coders[0]->data;

	i = 0;
	while(i < (*data).nb_coders){
		if (Coders[i])
		{
			coder = Coders[i];
			if(coder->left_dongle)
				free(coder->left_dongle);
			
			free(coder);
		}
		else
			break;
		i++;
	}
}
