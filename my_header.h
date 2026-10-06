#ifndef MY_HEADER
#define MY_HEADER

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


typedef struct Data{
	int nb_coders;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refactor;
	int nb_compiles;
	int dongle_cooldown;
	char* scheduler;
} Data;


typedef struct Dongle{
	int index;
	int available;
	pthread_mutex_t mutex;
}Dongle;


typedef struct Coder{
	int index;
	pthread_t thread;
	Data data;
	int time_from_last_compile;
	int compile_count;
} Coder;


typedef struct Heap{
	Coder **list;
	int size;
}Heap;


Data	create_data(int* list, char* scheduler);


Coder	**creating_coders(Data data, Dongle** Dongles, int* list);
Dongle	**creating_dongles(Data data, Coder** Coders, int *list);
void	free_coders(Coder **coders_list, Data data);
void	free_dongles(Dongle **dongles_list, Data data);
void	free_and_exit(Dongle **dongles,Coder** coders, Data data, int* list);


int*	parse(int ac, char* av[]);
void	parse_scheduler(char* str);
int		_atoi(const char* str, int nb);
int		ft_isalpha(int c);
int		ft_str_len(const char* str);
char*	str_tolower(char* str);
int		ft_cmp(const char* s1, const char* s2);

#endif