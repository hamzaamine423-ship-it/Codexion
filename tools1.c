#include "my_header.h"

void ft_putnbr(int nb)
{
	int res;
	char c;

	while (nb >= 10)
		ft_putnbr(nb / 10);
	c = (nb % 10) + 48;
	write(1, &c, 1);



}

// int main (){
// 	ft_putnbr(100);
// }


int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	ft_str_len(const char* str)
{
	int i;

	i = 0;
	while(str[i])
		i++;

	return i;
}

int ft_cmp(const char* s1, const char* s2)
{
	int i;

	if (ft_str_len(s1) != ft_str_len(s2))
		return 0;

	i = 0;
	while(s1[i]){
		if (s1[i] != s2[i])
			return 0;
		i++;
	}

	return 1;
}

char*	str_tolower(char* str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (str[i] <= 'Z' && str[i] >= 'A')
		{
			str[i] += 32;
		}

		i++;
	}

	return (str);
}
