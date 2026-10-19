#ifndef MY_HEADER
#define MY_HEADER

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

 

typedef struct Dongle Dongle;
typedef struct Heap Heap;
typedef struct Data Data;
typedef struct Coder Coder;

typedef struct Monitor{
	int burnout;
	pthread_t thread;	
} Monitor;

typedef struct Dongle{
	pthread_mutex_t d_mutex;
	int				index;
	int				taken;
	long released;
	int coolddown;
} Dongle;

typedef struct Queue{
	pthread_mutex_t q_mutex;
	pthread_cond_t q_cond;
	int front;
	int rear;
	int size;
	int capacity;
	Coder** Coders;
} Queue;

typedef struct Heap{
	pthread_cond_t h_cond;
	pthread_mutex_t h_mutex;
	Coder	**Coders;
	int		size;
} Heap;

typedef struct Data{
	pthread_mutex_t global_mutex;
	pthread_cond_t cond;
	int simulation_stop;
	Coder **Coders;
	int		nb_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		nb_compiles;
	int		dongle_cooldown;
	char*	scheduler;
	Heap	*heap;
	Queue	*queue;
	long start_time;
} Data;

typedef struct Coder{
	pthread_mutex_t c_mutex;
	Dongle			*left_dongle;
	Dongle			*right_dongle;
	int				index;
	pthread_t		thread;
	Data			*data;
	long			last_compile;
	int	compile_count;
} Coder;

int check_last_compile(Coder *coder);


// time functions:
long gettime(void);

// helper function:
void put_str_codexion(int nb, long time, char *str);
void ft_putstr(const char *str);
int		ft_isalpha(int c);
int		ft_str_len(const char* str);
char*	str_tolower(char* str);
int		ft_cmp(const char* s1, const char* s2);

// dongle functions:
int taking_dongles(Coder *coder);
int checks_takes_dongles(Coder *coder);
void recovery_dongles(Coder* coder);

// Creating functions:
Coder	**creating_coders(Data *data);
Data	create_data(int** list, char* scheduler);
Dongle*	create_dongle(int index, int dongle_cooldown);

// fifo_functions:
int until_i_am_top_fifo(Queue *queue, Coder *coder);
void check_queue(Queue queue);
Queue* init_queue(Data data);
Coder* pop_queue(Queue *queue);
void push_queue(Queue *queue, Coder *new_coder);
// edf_functions:
void check_heap(Heap heap);
Heap*	init_heap(Data data);
Coder*	pop_heap(Heap* heap);
int until_i_am_top_edf(Heap *heap, Coder *coder);
void	remove_top_heap(Heap* heap);
void	push_heap(Heap* heap, Coder* new_coder);

// start_functions:
void start_edf(Coder** Coders, Data data);
void start_fifo(Coder** Coders, Data data);


// Freing functions:
void	free_coders(Coder **coders_list);
void	free_and_exit(Coder** coders, Heap **heap);

// monitor:
Monitor* start_monitor(Coder **Coders, int time_to_burnout);
int checks_simulation(Coder *coder);


// Parsing functions:
int*	parse(int ac, char* av[]);
void	parse_scheduler(char* str);
int		_atoi(const char* str, int nb);


int	debugging(Coder *coder);
int	refactoring(Coder *coder);
int	compiling(Coder* coder);


#endif
