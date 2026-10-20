/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:33 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:53:46 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

int	taking_dongles(t_coder *cooder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		current_time;

	left = cooder->left_dongle;
	right = cooder->right_dongle;
	current_time = gettime();
	current_time -= cooder->datta->start_time;
	left->taken = 1;
	right->taken = 1;
	if (checks_simulation(cooder, "print"))
	{
		put_str_codexion(cooder->index, current_time, "has taken a dongle\n");
		put_str_codexion(cooder->index, current_time, "has taken a dongle\n");
		pthread_mutex_unlock(&cooder->datta->global_mutex);
	}
	return (1);
}

int	checks_takes_dongles(t_coder *cooder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		current_time;

	left = cooder->left_dongle;
	right = cooder->right_dongle;
	if (left->released == -1 && right->released == -1)
		return (taking_dongles(cooder));
	current_time = gettime();
	current_time -= cooder->datta->start_time;
	if (current_time > right->released + right->coolddown)
	{
		if (current_time > (left->released + left->coolddown))
			return (taking_dongles(cooder));
	}
	return (0);
}

void	recovery_dongles(t_coder *cooder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = cooder->left_dongle;
	right = cooder->right_dongle;
	left->taken = 0;
	right->taken = 0;
	pthread_mutex_unlock(&left->d_mutex);
	pthread_mutex_unlock(&right->d_mutex);
}
