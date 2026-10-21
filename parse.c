/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamine <hamine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 18:29:58 by hamine            #+#    #+#             */
/*   Updated: 2026/10/20 18:29:59 by hamine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion_header.h"

int	atoi_error(int index)
{
	printf("Error:\n    Argument number %d is incorrect !!\n", index);
	printf("        the argument should be a positive integer\n");
	return (-1);
}

int	_atoi(const char *str, int index)
{
	int		i;
	long	res;

	i = 0;
	res = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return (atoi_error(index));
		res = res * 10 + (str[i] - 48);
		if (res > 2147483647)
			return (atoi_error(index));
		i++;
	}
	return ((int)res);
}

int	parse_scheduler(char *str)
{
	if (!ft_cmp(str, "edf") && !ft_cmp(str, "fifo"))
	{
		printf("Error:\n");
		printf("    Type of the scheduler is incorrect !!\n");
		printf("        the available options are: 'edf' or 'fifo'\n");
		return (0);
	}
	return (1);
}

int	*parse(int ac, char *av[])
{
	int	*list;
	int	i;

	if (ac != 9)
	{
		printf("The number of the arguments should be 8.\n");
		return (NULL);
	}
	list = malloc(7 * sizeof(int));
	if (!list)
		return (NULL);
	i = 1;
	while (i < ac - 1)
	{
		if (_atoi(av[i], i) == -1)
		{
			free(list);
			return (NULL);
		}
		list[i - 1] = _atoi(av[i], i);
		i++;
	}
	if (!parse_scheduler(av[8]))
		return (NULL);
	return (list);
}
