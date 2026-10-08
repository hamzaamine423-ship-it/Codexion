#include "my_header.h"

void	creating_coders2(Data data, Coder *coder, int index)
{
	coder->data = data;
	coder->index = index;
	coder->compile_count = 0;
}

Coder	**creating_coders(Data data, int* list)
{
	Coder	**Coders_list;
	Coder *coder;
	int		i;
	Dongle *dongle_l;

	Coders_list = malloc(data.nb_coders * sizeof(Coder *));
	if (!Coders_list)
		free_and_exit(Coders_list, data, list);
	i = 1;
	while (i <= data.nb_coders) 
	{
		coder = malloc(sizeof(Coder));
		dongle_l = create_dongle(i);
		if (!coder || !dongle_l)
			free_and_exit(Coders_list, data, list);
		coder->left_dongle = dongle_l;
		coder->data = data;
		coder->index = i;
	
		if (i > 1)
			coder->right_dongle = Coders_list[i - 2]->left_dongle;
		Coders_list[i - 1] = coder;
		i++;
	}
	if (data.nb_coders > 1)
		Coders_list[0]->right_dongle = Coders_list[data.nb_coders - 1]->left_dongle;
	return Coders_list;
}

Dongle* create_dongle(int index)
{
	Dongle *dongle;

	dongle = malloc(sizeof(Dongle));
	if (!dongle)
		return NULL;
	
	pthread_mutex_init(&dongle->mutex, NULL);
	dongle->index = index;
	dongle->available = 1;

	return dongle;
}

Data create_data(int* list, char* scheduler)
{
	Data data;

	data.nb_coders = list[0];
	data.time_to_burnout = list[1] / 1000;
	data.time_to_compile = list[2] / 1000;
	data.time_to_debug = list[3] / 1000;
	data.time_to_refactor = list[4] / 1000;
	data.nb_compiles = list[5];
	data.dongle_cooldown = list[6];

	data.scheduler = str_tolower(scheduler);

	return data;
}
