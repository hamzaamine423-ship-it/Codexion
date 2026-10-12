#include "my_header.h"


int check_burnout(Coder** Coders)
{
	Data *data;
	int i;
	size_t current_time;
	unsigned long burnout;
    unsigned long   time_between_compile;
	
	data = Coders[0]->data;
	burnout = data->time_to_burnout;
	i = 0;
	current_time = gettime();
    current_time -= data->start_time;
	while (i < data->nb_coders)
	{
        if (Coders[i]->compile_count == data->nb_compiles || !Coders[i]->last_compile)
            continue;
    	if (current_time > Coders[i]->last_compile)
		{
	    	put_str_codexion(Coders[i]->index, current_time, "burn out\n");
			// free_and_exit(Coders, heap);
			exit(0);
	    }

		i++;
	}
	return 0;
}


int check_compiling(Coder **Coders)
{
	int compile_count;
	Data *data;
	int i;

	data = Coders[0]->data;
	compile_count = data->nb_compiles;

	i = 0;
	while (i < data->nb_coders)
	{
		if (compile_count != Coders[i]->compile_count)
			return 0;
		i++;
	}

	return 1;

}


void* monitor_check(void *arg)
{
	Heap** heap;
	Coder **Coders;

	Coders = (Coder **)arg;
	heap = &Coders[0]->data->heap;
	while (!check_compiling(Coders))
	{
		check_burnout(Coders);

		usleep(1000);
	}

	return NULL;
}


void start_monitor(Coder **Coders, int burnout)
{
	Monitor monitor;

    monitor.burnout = burnout;
	pthread_create(&monitor.thread, NULL, &monitor_check, Coders);

}

