#include "codexion_header.h"

void	creating_coders2(t_data *datta, t_coder *cooder,
						int index, t_dongle *left_d)
{
	cooder->mutex_initialized = pthread_mutex_init(&cooder->c_mutex, NULL);
	if (cooder->mutex_initialized != 0)
	{
		free_and_exit(datta);
	}
	cooder->left_dongle = left_d;
	cooder->datta = datta;
	cooder->index = index;
	cooder->compile_count = 0;
	cooder->last_compile = -1;
}

t_coder	**creating_coders(t_data *datta)
{
	t_coder		**cooders_list;
	t_coder		*cooder;
	t_dongle	*dongle_l;
	int			i;

	cooders_list = malloc((*datta).nb_coders * sizeof(t_coder *));
	if (!cooders_list)
		return (NULL);
	i = 1;
	while (i <= (*datta).nb_coders)
	{
		cooder = malloc(sizeof(t_coder));
		dongle_l = create_dongle(i, datta->dongle_cooldown);
		if (!cooder || !dongle_l)
			free_and_exit(datta);
		creating_coders2(datta, cooder, i, dongle_l);
		if (i > 1)
			cooder->right_dongle = cooders_list[i - 2]->left_dongle;
		cooders_list[i - 1] = cooder;
		i++;
	}
	if ((*datta).nb_coders > 1)
		cooders_list[0]->right_dongle = cooders_list[(*datta).nb_coders
			- 1]->left_dongle;
	return (cooders_list);
}

t_dongle	*create_dongle(int index, int dongle_cooldown)
{
	t_dongle	*doongle;

	doongle = malloc(sizeof(t_dongle));
	if (!doongle)
		return (NULL);
	doongle->mutex_initialized = pthread_mutex_init(&doongle->d_mutex, NULL);
	if (doongle->mutex_initialized != 0)
	{
		free(doongle);
		return (NULL);
	}
	doongle->index = index;
	doongle->taken = 0;
	doongle->released = -1;
	doongle->coolddown = dongle_cooldown;
	return (doongle);
}

t_data	create_data(int **list, char *scheduler)
{
	t_data	datta;
	int		*cp_list;

	cp_list = *list;
	datta.heapp = NULL;
	datta.queuee = NULL;
	datta.moonitor = NULL;
	datta.simulation_stop = 0;
	datta.nb_coders = cp_list[0];
	datta.time_to_burnout = cp_list[1];
	datta.time_to_compile = cp_list[2];
	datta.time_to_debug = cp_list[3];
	datta.time_to_refactor = cp_list[4];
	datta.nb_compiles = cp_list[5];
	datta.dongle_cooldown = cp_list[6];
	datta.scheduler = scheduler;
	free(*list);
	*list = NULL;
	if (!datta.nb_coders)
		exit(0);
	datta.mutex_initialized = pthread_mutex_init(&datta.global_mutex, NULL);
	datta.cond_initialized = pthread_cond_init(&datta.cond, NULL);
	if (datta.cond_initialized != 0 || datta.mutex_initialized != 0)
		exit(0);
	return (datta);
}
