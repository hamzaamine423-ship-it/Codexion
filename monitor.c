#include "my_header.h"


int check_burnout(Coder** Coders)
{
	Data *data;
	int j;
	size_t current_time;
	
	data = Coders[0]->data;
	j = 0;
	while (j < data->nb_coders)
	{
		current_time = gettime();
    	current_time -= data->start_time;
		pthread_mutex_lock(&data->global_mutex);
        if (Coders[j]->compile_count == data->nb_compiles || !Coders[j]->last_compile)
        {
			j++;
			pthread_mutex_unlock(&data->global_mutex);
			continue;
		}
    	if (current_time > Coders[j]->last_compile)
		{
	    	put_str_codexion(Coders[j]->index, current_time, "burn out\n");
			// free_and_exit(Coders, heap);
			return 0;
	    }
		pthread_mutex_unlock(&data->global_mutex);
		j++;
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


void finish_monitor(Monitor *monitor)
{
	pthread_join(monitor->thread, NULL);
}

Monitor* start_monitor(Coder **Coders, int burnout)
{
	Monitor *monitor;

	monitor = malloc(sizeof(Monitor));
	if(!monitor)
		exit(0);

    monitor->burnout = burnout;
	pthread_create(&monitor->thread, NULL, &monitor_check, Coders);

	return monitor;
}

