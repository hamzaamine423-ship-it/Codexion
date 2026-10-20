/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:47 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:29:48 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

void	free_heap(t_heap *heapp)
{
	if (heapp->mutex_initialized)
		pthread_mutex_destroy(&heapp->h_mutex);
	if (heapp->cond_initialized)
		pthread_cond_destroy(&heapp->h_cond);
	free(heapp->coders);
	heapp->coders = NULL;
	free(heapp);
	heapp = NULL;
}

void	free_queue(t_queue *queuee)
{
	if (queuee->mutex_initialized)
		pthread_mutex_destroy(&queuee->q_mutex);
	if (queuee->cond_initialized)
		pthread_cond_destroy(&queuee->q_cond);
	free(queuee->coders);
	queuee->coders = NULL;
	free(queuee);
	queuee = NULL;
}

void	free_coders(t_coder **coders)
{
	int		i;
	t_data	*datta;
	t_coder	*coder;

	datta = coders[0]->datta;
	i = 0;
	while (i < (*datta).nb_coders)
	{
		if (coders[i])
		{
			coder = coders[i];
			if (coder->left_dongle)
			{
				if (coder->left_dongle->mutex_initialized)
					pthread_mutex_destroy(&coder->left_dongle->d_mutex);
				free(coder->left_dongle);
			}
			free(coder);
		}
		else
			break ;
		i++;
	}
	free(coders);
}

void	free_and_exit(t_data *datta)
{
	if (datta->coders)
		free_coders(datta->coders);
	if (ft_cmp(datta->scheduler, "edf"))
	{
		if (datta->heapp)
			free_heap(datta->heapp);
	}
	else
	{
		if (datta->queuee)
			free_queue(datta->queuee);
	}
	if (datta->mutex_initialized)
		pthread_mutex_destroy(&datta->global_mutex);
	if (datta->cond_initialized)
		pthread_cond_destroy(&datta->cond);
	if (datta->moonitor)
		free(datta->moonitor);
	exit(0);
}
