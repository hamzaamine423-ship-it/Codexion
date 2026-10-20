#include "codexion_header.h"

t_heap	*init_heap(t_data datta)
{
	t_heap	*heapp;

	heapp = malloc(sizeof(t_heap));
	if (!heapp)
		return (NULL);
	heapp->size = 0;
	heapp->coders = malloc(sizeof(t_coder *) * datta.nb_coders);
	if (!heapp->coders)
	{
		free(heapp);
		return (NULL);
	}
	heapp->cond_initialized = pthread_cond_init(&heapp->h_cond, NULL);
	heapp->mutex_initialized = pthread_mutex_init(&heapp->h_mutex, NULL);
	if (heapp->mutex_initialized != 0 || heapp->cond_initialized != 0)
	{
		free(heapp);
		return (NULL);
	}
	return (heapp);
}

void	push_heap2(t_heap *heapp, int child_i)
{
	t_coder	*parent;
	t_coder	*child;
	int		parent_i;

	while (1)
	{
		parent_i = child_i / 2;
		parent = heapp->coders[parent_i - 1];
		child = heapp->coders[child_i - 1];
		if (parent->last_compile > child->last_compile)
		{
			heapp->coders[parent_i - 1] = child;
			heapp->coders[child_i - 1] = parent;
			child_i = parent_i;
			if (child_i <= 1)
				break ;
		}
		else
			break ;
	}
}

void	push_heap(t_heap *heapp, t_coder *new_coder)
{
	int	child_i;

	if (!heapp->size)
	{
		heapp->coders[0] = new_coder;
		heapp->size++;
		return ;
	}
	heapp->coders[heapp->size] = new_coder;
	heapp->size++;
	child_i = heapp->size;
	push_heap2(heapp, child_i);
}
