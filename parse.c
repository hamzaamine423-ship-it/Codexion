#include <stdio.h>
#include <unistd.h>
#include "my_header.h"

int _atoi(const char* str, int nb)
{
	int i;
	long res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (!(str[i] >= 48 && str[i] <= 57) || (res > 2147483647))
		{
			res = 0;
			break;
		}
		res = res * 10 + (str[i] - 48);
		i++;
	}
	if (res == 0)
	{
		printf("Error:\n    Argument number %d is incorrect !!\n", nb);
		printf("        the argument should be a positive integer\n");
	}
	return ((int)res);
}

void parse_scheduler(char* str)
{
	str = str_tolower(str);
	if (!ft_cmp(str, "edf") && !ft_cmp(str, "fifo")){
		printf("Error:\n");
		printf("    Type of the scheduler is incorrect !!\n");
		printf("        the available options are: 'edf' or 'fifo'\n");
		exit(0);
	}
}

int* parse(int ac, char* av[])
{
	int *list;
	int i;

	if (ac != 9)
	{
		printf("The number of the arguments should be 8.\n");
		exit(0);
	}
	list = malloc(7 * sizeof(int));
	if (!list)
		return NULL;

	i = 1;
	while(i < ac - 1)
	{
		if (!_atoi(av[i], i))
		{
			free(list);
			exit(0);
		}
		list[i - 1] = _atoi(av[i], i);
		i++;
	}
	parse_scheduler(av[8]);
	return list;
}
