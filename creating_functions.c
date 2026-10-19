#include "codexion_header.h"


void	creating_coders2(Data *data, Coder *coder, int index)
{
	coder->mutex_initialized = pthread_mutex_init(&coder->c_mutex, NULL);
	if (coder->mutex_initialized != 0)
	{
		free_and_exit(data);
	}

	coder->data = data;
	coder->index = index;
	coder->compile_count = 0;
	coder->last_compile = -1;
}

Coder	**creating_coders(Data *data)
{
	Coder	**Coders_list;
	Coder *coder;
	int		i;
	Dongle *dongle_l;

	Coders_list = malloc((*data).nb_coders * sizeof(Coder *));
	if (!Coders_list)
		return NULL;
	i = 1;
	while (i <= (*data).nb_coders) 
	{
		coder = malloc(sizeof(Coder));
		dongle_l = create_dongle(i, data->dongle_cooldown);
		if (!coder || !dongle_l)
			free_and_exit(data);
		coder->left_dongle = dongle_l;
		creating_coders2(data, coder, i);	
		if (i > 1)
			coder->right_dongle = Coders_list[i - 2]->left_dongle;
		Coders_list[i - 1] = coder;
		i++;
	}
	if ((*data).nb_coders > 1)
		Coders_list[0]->right_dongle = Coders_list[(*data).nb_coders - 1]->left_dongle;
	return Coders_list;
}

Dongle* create_dongle(int index, int dongle_cooldown)
{
	Dongle *dongle;

	dongle = malloc(sizeof(Dongle));
	if (!dongle)
		return NULL;
	dongle->mutex_initialized = pthread_mutex_init(&dongle->d_mutex, NULL);

	if (dongle->mutex_initialized != 0)
	{
		free(dongle);
		return NULL;
	}
	dongle->index = index;
	dongle->taken = 0;
	dongle->released = -1;
	dongle->coolddown = dongle_cooldown;

	return dongle;
}

Data create_data(int** list, char* scheduler)
{
	Data data;
	int *cp_list;

	cp_list = *list;
	data.heap = NULL;
	data.queue = NULL;
	data.monitor = NULL;
	data.simulation_stop = 0;
	data.nb_coders = cp_list[0];
	data.time_to_burnout = cp_list[1];
	data.time_to_compile = cp_list[2];
	data.time_to_debug = cp_list[3];
	data.time_to_refactor = cp_list[4];
	data.nb_compiles = cp_list[5];
	data.dongle_cooldown = cp_list[6];
	data.scheduler = scheduler;
	free(*list);
	*list = NULL;
	if (!data.nb_coders)
		exit(0);
	data.mutex_initialized = pthread_mutex_init(&data.global_mutex, NULL);
	data.cond_initialized = pthread_cond_init(&data.cond, NULL);
	if (data.cond_initialized != 0 || data.mutex_initialized != 0)
		exit(0);
	return data;
}
