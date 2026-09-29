#ifndef MY_HEADER
#define MY_HEADER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int* parse(int ac, char* av[]);
void parse_scheduler(char* str);
long _atoi(const char* str, int nb);
int is_valid_integer(char* str);
int	ft_isalpha(int c);
int ft_str_len(const char* str);
char* str_tolower(char* str);
int ft_cmp(const char* s1, const char* s2);

#endif