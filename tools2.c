#include "codexion_header.h"



int convert_size_t(size_t time, char *new_str)
{
	char tmp[20];
	int i;
	int j;

	i = 0;
	if (time == 0)
    	tmp[i++] = '0';
	while (time > 0)
	{
		tmp[i++] = (time % 10) + '0';
		time /= 10;
	}

	j = 0;
	while(i > 0)
		new_str[j++] = tmp[--i];
	
	new_str[j] = ' ';

	return ++j;
}

int convert_int(int index, int nb,char *new_str)
{
	char tmp[12];
	int i;
	int j;

	i = 0;
	while (nb > 0)
	{
		tmp[i++] = (nb % 10) + '0';
		nb /= 10;
	}

	j = index;
	while(i > 0)
		new_str[j++] = tmp[--i];
	
	new_str[j] = ' ';

	return ++j;
}

void put_str_codexion(int nb, size_t time, char *str)
{
	char new_str[50];
	int index;
	int i;
	int len;

	index = convert_size_t(time, new_str);
	index = convert_int(index, nb, new_str);

	i = 0;
	while (str[i])
		new_str[index++] = str[i++];

	new_str[index] = '\0';

	len = ft_str_len(new_str);

	write(1, &new_str[0], len);
}


void ft_putstr(const char *str)
{
	int len;

	len = ft_str_len(str);

	write(1, &str[0], len);
}
