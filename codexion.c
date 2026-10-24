/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:52 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:29:53 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

t_monitor	*start_monitor(t_coder **coders, t_data *data)
{
	t_monitor	*moonitor;

	moonitor = malloc(sizeof(t_monitor));
	if (!moonitor)
		return (free_all(data));
	moonitor->burnout = data->time_to_burnout;
	if (pthread_create(&moonitor->thread, NULL, &monitor_check, coders) != 0)
		return (free_all(data));
	return (moonitor);
}

int	start_function(t_data *data)
{
	t_heap	*heapp;
	t_queue	*queuee;

	if (ft_cmp(data->scheduler, "edf"))
	{
		heapp = init_heap(*data);
		if (!heapp)
			return (0);
		data->heapp = heapp;
		if (!start_edf(data->coders, data))
			return (0);
	}
	else
	{
		queuee = init_queue(*data);
		if (!queuee)
			return (0);
		data->queuee = queuee;
		if (!start_fifo(data->coders, data))
			return (0);
	}
	return (1);
}

int	main(int ac, char *av[])
{
	t_data		data;
	t_coder		**coders;
	t_monitor	*moonitor;

	data = create_data(ac, av);
	if (!validate_data(data))
		return (0);
	coders = creating_coders(&data);
	if (!coders)
		return (0);
	data.coders = coders;
	data.start_time = gettime();
	moonitor = start_monitor(coders, &data);
	if (!moonitor)
		return (0);
	data.moonitor = moonitor;
	start_function(&data);
	free_all(&data);
	return (0);
}
