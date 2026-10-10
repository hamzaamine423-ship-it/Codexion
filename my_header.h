#ifndef MY_HEADER
#define MY_HEADER

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>


typedef struct Dongle Dongle;
typedef struct Heap Heap;
typedef struct Data Data;
typedef struct Coder Coder;

typedef struct Dongle{
	int				index;
	int				available;
	pthread_mutex_t	mutex;
}Dongle;

typedef struct Heap{
	Coder	**Coders;
	int		size;
}Heap;

typedef struct Data{
	int		nb_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		nb_compiles;
	int		dongle_cooldown;
	char*	scheduler;
	Heap	*heap;
} Data;

typedef struct Coder{
	Dongle		*left_dongle;
	Dongle		*right_dongle;
	int			index;
	pthread_t	thread;
	Data		*data;
	int			time_from_last_compile;
	int			compile_count;
} Coder;


// Creating functions:
Coder	**creating_coders(Data *data);
Data	create_data(int** list, char* scheduler);
Dongle*	create_dongle(int index);

// heap_functions:
Coder*	pop_heap(Heap* heap);
void	push_heap(Heap* heap, Coder* new_coder);
Heap*	create_heap_FIFO(Data data);
void	remove_top_heap(Heap* heap);

// Freing functions:
void	free_coders(Coder **coders_list);
void	free_and_exit(Coder** coders, Heap **heap);

// Parsing functions:
int*	parse(int ac, char* av[]);
void	parse_scheduler(char* str);
int		_atoi(const char* str, int nb);
int		ft_isalpha(int c);
int		ft_str_len(const char* str);
char*	str_tolower(char* str);
int		ft_cmp(const char* s1, const char* s2);


void	debugging(Coder coder);
void	refactoring(Coder coder);
void	compiling(Coder* coder);


#endif
