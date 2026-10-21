/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creating_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:31 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:29:32 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

int	creating_coders2(t_data *datta, t_coder *cooder, int index)
{
	t_dongle	*left_d;

	left_d = create_dongle(index, datta->dongle_cooldown);
	if (!left_d)
		return (0);
	cooder->left_dongle = left_d;
	cooder->mutex_initialized = pthread_mutex_init(&cooder->c_mutex, NULL);
	if (cooder->mutex_initialized != 0)
		return (0);
	cooder->left_dongle = left_d;
	cooder->datta = datta;
	cooder->index = index;
	cooder->compile_count = 0;
	cooder->last_compile = -1;
	return (1);
}

t_coder	**creating_coders(t_data *datta)
{
	t_coder	**cooders_list;
	t_coder	*cooder;
	int		i;

	cooders_list = malloc((*datta).nb_coders * sizeof(t_coder *));
	if (!cooders_list)
		return (NULL);
	i = 1;
	while (i <= (*datta).nb_coders)
	{
		cooder = malloc(sizeof(t_coder));
		if (!cooder)
			return (free_all(datta));
		if (!creating_coders2(datta, cooder, i))
			return (free_all(datta));
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
		return (NULL);
	doongle->index = index;
	doongle->taken = 0;
	doongle->released = -1;
	doongle->coolddown = dongle_cooldown;
	return (doongle);
}

int	validate_data(t_data data)
{
	if (!data.nb_coders)
		return (0);
	if (data.cond_initialized != 0 || data.mutex_initialized != 0)
		return (0);
	return (1);
}

t_data	create_data(int ac, char *av[])
{
	t_data	datta;
	int		*list;

	list = parse(ac, av);
	if (!list)
	{
		datta.nb_coders = 0;
		return (datta);
	}
	datta.heapp = NULL;
	datta.queuee = NULL;
	datta.moonitor = NULL;
	datta.simulation_stop = 0;
	datta.nb_coders = list[0];
	datta.time_to_burnout = list[1];
	datta.time_to_compile = list[2];
	datta.time_to_debug = list[3];
	datta.time_to_refactor = list[4];
	datta.nb_compiles = list[5];
	datta.dongle_cooldown = list[6];
	datta.scheduler = av[7];
	free(list);
	datta.mutex_initialized = pthread_mutex_init(&datta.global_mutex, NULL);
	datta.cond_initialized = pthread_cond_init(&datta.cond, NULL);
	return (datta);
}
