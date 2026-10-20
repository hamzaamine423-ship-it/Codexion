/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:39 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:55:41 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

int	take_dongles_edf2(t_coder *cooder)
{
	if (cooder->index == 1)
	{
		pthread_mutex_lock(&cooder->left_dongle->d_mutex);
		pthread_mutex_lock(&cooder->right_dongle->d_mutex);
	}
	else
	{
		pthread_mutex_lock(&cooder->right_dongle->d_mutex);
		pthread_mutex_lock(&cooder->left_dongle->d_mutex);
	}
	while (1)
	{
		if (!checks_simulation(cooder, ""))
		{
			pthread_mutex_unlock(&cooder->right_dongle->d_mutex);
			pthread_mutex_unlock(&cooder->left_dongle->d_mutex);
			return (0);
		}
		if (checks_takes_dongles(cooder))
			break ;
		usleep(5);
	}
	return (1);
}

int	take_dongles_edf(t_heap *heapp, t_coder *cooder)
{
	while (1)
	{
		pthread_mutex_lock(&heapp->h_mutex);
		if (!checks_simulation(cooder, ""))
		{
			pthread_mutex_unlock(&heapp->h_mutex);
			return (0);
		}
		if (heapp->size && heapp->coders[0] == cooder)
		{
			pop_heap(heapp);
			pthread_mutex_unlock(&heapp->h_mutex);
			break ;
		}
		else
		{
			pthread_mutex_unlock(&heapp->h_mutex);
			usleep(10);
		}
	}
	return (take_dongles_edf2(cooder));
}

int	fix_left(t_heap *heapp, int i, t_coder *cur_cooder)
{
	t_coder	*ch_left;

	if (i * 2 > heapp->size)
		return (0);
	ch_left = heapp->coders[i * 2 - 1];
	if (ch_left->last_compile < cur_cooder->last_compile)
	{
		heapp->coders[i - 1] = ch_left;
		heapp->coders[i * 2 - 1] = cur_cooder;
		return (1);
	}
	return (0);
}

int	fix_right(t_heap *heapp, int i, t_coder *cur_cooder)
{
	t_coder	*ch_right;

	if (i * 2 + 1 > heapp->size)
		return (0);
	ch_right = heapp->coders[i * 2];
	if (ch_right->last_compile < cur_cooder->last_compile)
	{
		heapp->coders[i - 1] = ch_right;
		heapp->coders[i * 2] = cur_cooder;
		return (1);
	}
	return (0);
}

void	fix_heap(t_heap *heapp)
{
	t_coder	*cur_cooder;
	int		i;

	i = 1;
	while (i < heapp->size)
	{
		cur_cooder = heapp->coders[i - 1];
		if (fix_left(heapp, i, cur_cooder))
		{
			if (i - 1 > 1)
				i -= 1;
			else
				i = 1;
			continue ;
		}
		if (fix_right(heapp, i, cur_cooder))
		{
			if (i - 2 > 1)
				i -= 2;
			else
				i = 1;
			continue ;
		}
		i++;
	}
}
