/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:41 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:57:49 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

void	*go_fifo(void *arg)
{
	t_coder	*cooder;
	t_data	*datta;
	t_queue	*queuee;

	cooder = (t_coder *)arg;
	datta = cooder->datta;
	queuee = cooder->datta->queuee;
	while (cooder->compile_count < datta->nb_compiles)
	{
		if (!checks_simulation(cooder, ""))
			break ;
		pthread_mutex_lock(&queuee->q_mutex);
		push_queue(queuee, cooder);
		pthread_mutex_unlock(&queuee->q_mutex);
		if (!take_dongles_fifo(queuee, cooder))
			break ;
		if (!compiling(cooder))
			break ;
	}
	return (NULL);
}

int	start_fifo(t_coder **coders, t_data *datta)
{
	int		i;
	t_coder	*cooder;

	i = 0;
	if (!one_coder_case(coders[0]))
		return (0);
	while (i < (*datta).nb_coders)
	{
		cooder = coders[i];
		if (pthread_create(&cooder->thread, NULL, &go_fifo, cooder) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < (*datta).nb_coders)
	{
		cooder = coders[i];
		if (pthread_join(cooder->thread, NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_join((*datta).moonitor->thread, NULL) != 0)
		return (0);
	return (1);
}
