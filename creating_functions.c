#include "my_header.h"

Coder	**creating_coders(Data data, Dongle** Dongles, int* list)
{
	Coder	**Coders_list;
	Coder *coder;
	int		i;

	Coders_list = malloc(data.nb_coders * sizeof(Coder *));
	if (!Coders_list)
		free_and_exit(Dongles, Coders_list, data, list);

	i = 1;
	while (i <= data.nb_coders)
	{
		coder = malloc(sizeof(Coder));
		if (!coder)
			free_and_exit(Dongles, Coders_list, data, list);

		coder->data = data;
		coder->index = i;

		Coders_list[i - 1] = coder;
		i++;
	}

	return Coders_list;
}

Dongle** creating_dongles(Data data, Coder** Coders, int *list)
{
	Dongle **Dongles_list;
	int i;
	Dongle *dongle;

	Dongles_list = malloc(sizeof(Dongle *) * data.nb_coders);
	if (!Dongles_list)
		free_and_exit(Dongles_list, Coders, data, list);

	i = 1;
	while ( i <= data.nb_coders)
	{
		dongle = malloc(sizeof(Dongle));
		if(!dongle)
			free_and_exit(Dongles_list, Coders, data, list);

		dongle->index = i;
		pthread_mutex_init(&dongle->mutex, NULL);

		dongle->available = 1;
		Dongles_list[i - 1] = dongle;
		i++;
	}

	return Dongles_list;
}


Data create_data(int* list, char* scheduler)
{
	Data data;

	data.nb_coders = list[0];
	data.nb_compiles = list[1];
	data.time_to_burnout = list[2];
	data.time_to_compile = list[3];
	data.time_to_debug = list[4];
	data.time_to_refactor = list[5];
	data.dongle_cooldown = list[6];

	data.scheduler = str_tolower(scheduler);

	return data;
}
