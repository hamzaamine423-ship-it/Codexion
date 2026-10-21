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
	if (heapp->coders)
	{
		free(heapp->coders);
		heapp->coders = NULL;
	}
	free(heapp);
	heapp = NULL;
}

void	free_queue(t_queue *queuee)
{
	if (queuee->mutex_initialized)
		pthread_mutex_destroy(&queuee->q_mutex);
	if (queuee->cond_initialized)
		pthread_cond_destroy(&queuee->q_cond);
	if (queuee->coders)
	{
		free(queuee->coders);
		queuee->coders = NULL;
	}
	free(queuee);
	queuee = NULL;
}

void	free_coders(t_coder **coders, t_data *data)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < data->nb_coders)
	{
		if (coders[i])
		{
			coder = coders[i];
			if (coder->left_dongle)
			{
				if (coder->left_dongle->mutex_initialized)
					pthread_mutex_destroy(&coder->left_dongle->d_mutex);
				free(coder->left_dongle);
				coder->left_dongle = NULL;
			}
			free(coder);
			coder = NULL;
		}
		else
			break ;
		i++;
	}
	free(coders);
	coders = NULL;
}

void	*free_all(t_data *datta)
{
	if (datta->coders)
		free_coders(datta->coders, datta);
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
	{
		free(datta->moonitor);
		datta->moonitor = NULL;
	}
	return (NULL);
}
