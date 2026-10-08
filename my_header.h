#ifndef MY_HEADER
#define MY_HEADER

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>




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
	Dongle *left_dongle;
	Dongle *right_dongle;
	int index;
	pthread_t thread;
	Data data;
	int time_from_last_compile;
	int compile_count;
} Coder;


typedef struct Heap{
	Coder **Coders;
	int size;
}Heap;

Dongle* create_dongle(int index);

void fix_top(Heap *heap);
void remove_top_heap(Heap* heap);

Heap* create_heap_FIFO(Data data);
Data	create_data(int* list, char* scheduler);


Coder	**creating_coders(Data data, int* list);
void	free_coders(Coder **coders_list, Data data);
void	free_and_exit(Coder** coders, Data data, int* list);


int*	parse(int ac, char* av[]);
void	parse_scheduler(char* str);
int		_atoi(const char* str, int nb);
int		ft_isalpha(int c);
int		ft_str_len(const char* str);
char*	str_tolower(char* str);
int		ft_cmp(const char* s1, const char* s2);



void debugging(Coder coder);
void refactoring(Coder coder);
void* compiling(void *arg);


#endif