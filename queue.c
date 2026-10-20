/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:30:03 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:30:04 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

t_queue	*init_queue(t_data datta)
{
	t_queue	*queuee;

	queuee = malloc(sizeof(t_queue));
	if (!queuee)
		return (NULL);
	queuee->front = 0;
	queuee->rear = 0;
	queuee->size = 0;
	queuee->capacity = datta.nb_coders;
	queuee->coders = malloc(sizeof(t_coder *) * datta.nb_coders);
	if (!queuee->coders)
	{
		free(queuee);
		return (NULL);
	}
	queuee->mutex_initialized = pthread_mutex_init(&queuee->q_mutex, NULL);
	queuee->cond_initialized = pthread_cond_init(&queuee->q_cond, NULL);
	if (queuee->mutex_initialized != 0 || queuee->cond_initialized != 0)
	{
		free(queuee);
		return (NULL);
	}
	return (queuee);
}

t_coder	*pop_queue(t_queue *queuee)
{
	t_coder	*coder;

	if (!queuee->size)
		return (NULL);
	coder = queuee->coders[queuee->front];
	queuee->front = (queuee->front + 1) % queuee->capacity;
	queuee->size--;
	return (coder);
}

void	push_queue(t_queue *queuee, t_coder *new_coder)
{
	if (queuee->size == queuee->capacity)
		return ;
	queuee->coders[queuee->rear] = new_coder;
	queuee->rear = (queuee->rear + 1) % queuee->capacity;
	queuee->size++;
}
