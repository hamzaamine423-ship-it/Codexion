#ifndef CODEXION_HEADER
# define CODEXION_HEADER

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>

typedef struct dongle	t_dongle;
typedef struct monitor	t_monitor;
typedef struct heap		t_heap;
typedef struct data		t_data;
typedef struct coder	t_coder;

typedef struct monitor
{
	int					burnout;
	pthread_t			thread;
}						t_monitor;

typedef struct dongle
{
	pthread_mutex_t		d_mutex;
	int					mutex_initialized;
	int					index;
	int					taken;
	long				released;
	int					coolddown;
}						t_donlge;

typedef struct queue
{
	pthread_mutex_t		q_mutex;
	int					mutex_initialized;
	pthread_cond_t		q_cond;
	int					cond_initialized;
	int					front;
	int					rear;
	int					size;
	int					capacity;
	t_coder				**coders;
}						t_queue;

typedef struct heap
{
	pthread_cond_t		h_cond;
	int					cond_initialized;
	pthread_mutex_t		h_mutex;
	int					mutex_initialized;
	t_coder				**coders;
	int					size;
}						t_heap;

typedef struct data
{
	pthread_mutex_t		global_mutex;
	int					mutex_initialized;
	pthread_cond_t		cond;
	int					cond_initialized;
	int					simulation_stop;
	t_coder				**coders;
	int					nb_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					nb_compiles;
	int					dongle_cooldown;
	char				*scheduler;
	t_heap				*heapp;
	t_queue				*queuee;
	long				start_time;
	t_monitor			*moonitor;
}						t_data;

typedef struct coder
{
	pthread_mutex_t		c_mutex;
	int					mutex_initialized;
	t_donlge			*left_dongle;
	t_donlge			*right_dongle;
	int					index;
	pthread_t			thread;
	t_data				*datta;
	long				last_compile;
	int					compile_count;
}						t_coder;

// time functions:
long					gettime(void);
int						ft_sleep(int sleep_time, t_coder *cooder,
							int in_compile);

// helper function:
void					put_str_codexion(int nb, long time, char *str);
void					ft_putstr(const char *str);
int						ft_isalpha(int c);
int						ft_str_len(const char *str);
int						ft_cmp(const char *s1, const char *s2);

// dongle functions:
int						taking_dongles(t_coder *cooder);
int						checks_takes_dongles(t_coder *cooder);
void					recovery_dongles(t_coder *cooder);

// Creating functions:
t_coder					**creating_coders(t_data *datta);
t_data					create_data(int **list, char *scheduler);
t_donlge				*create_dongle(int index, int dongle_cooldown);

// fifo_functions:
int						take_dongles_fifo(t_queue *queuee, t_coder *cooder);
void					check_queue(t_queue queuee);
t_queue					*init_queue(t_data datta);
t_coder					*pop_queue(t_queue *queuee);
void					push_queue(t_queue *queuee, t_coder *new_coder);
void					remove_top_heap(t_heap *heapp);
void					fix_heap(t_heap *heapp);

// edf_functions:
void					check_heap(t_heap heapp);
t_heap					*init_heap(t_data datta);
t_coder					*pop_heap(t_heap *heapp);
int						take_dongles_edf(t_heap *heapp, t_coder *cooder);

void					remove_top_heap(t_heap *heapp);
void					push_heap(t_heap *heapp, t_coder *new_coder);

// start_functions:
void					start_edf(t_coder **coders, t_data datta);
void					start_fifo(t_coder **coders, t_data datta);

// Freing functions:
void					free_coders(t_coder **coders_list);
void					free_and_exit(t_data *datta);

// monitor:
t_monitor				*start_monitor(t_coder **coders, int time_to_burnout,
							t_data *datta);
int						checks_simulation(t_coder *cooder);
void					*monitor_check(void *arg);

// Parsing functions:
int						*parse(int ac, char *av[]);
void					parse_scheduler(char *str);
int						_atoi(const char *str, int nb);

int						debugging(t_coder *cooder);
int						refactoring(t_coder *cooder);
int						compiling(t_coder *cooder);

#endif
