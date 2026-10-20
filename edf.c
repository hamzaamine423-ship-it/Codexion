/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 19:18:04 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 19:18:05 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

void	*go_edf(void *arg)
{
	t_coder	*cooder;
	t_data	*datta;
	t_heap	*heapp;

	cooder = (t_coder *)arg;
	datta = cooder->datta;
	heapp = cooder->datta->heapp;
	while (cooder->compile_count < datta->nb_compiles)
	{
		if (!checks_simulation(cooder, ""))
			break ;
		pthread_mutex_lock(&heapp->h_mutex);
		push_heap(heapp, cooder);
		pthread_mutex_unlock(&heapp->h_mutex);
		if (!take_dongles_edf(heapp, cooder))
			break ;
		if (!compiling(cooder))
			break ;
	}
	return (NULL);
}

void	start_edf(t_coder **coders, t_data datta)
{
	int		i;
	t_coder	*cooder;

	i = 0;
	if (coders[0]->datta->nb_coders == 1)
	{
		while (1)
		{
			if (!checks_simulation(coders[0], ""))
				return ;
		}
	}
	while (i < datta.nb_coders)
	{
		cooder = coders[i];
		pthread_create(&cooder->thread, NULL, &go_edf, cooder);
		i++;
	}
	i = 0;
	while (i < datta.nb_coders)
	{
		cooder = coders[i];
		pthread_join(cooder->thread, NULL);
		i++;
	}
}

void	remove_top_heap(t_heap *heapp)
{
	int	len;

	len = heapp->size;
	if (heapp->size == 1)
	{
		heapp->size--;
		return ;
	}
	heapp->coders[0] = heapp->coders[len - 1];
	heapp->size--;
	fix_heap(heapp);
}

t_coder	*pop_heap(t_heap *heapp)
{
	t_coder	*top_cooder;

	if (!heapp->size)
		return (NULL);
	top_cooder = heapp->coders[0];
	remove_top_heap(heapp);
	return (top_cooder);
}
