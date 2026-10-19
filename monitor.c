#include "codexion_header.h"


int checks_simulation(Coder *coder)
{
	pthread_mutex_lock(&coder->data->global_mutex);
	if (coder->data->simulation_stop == 1)
	{
		pthread_mutex_unlock(&coder->data->global_mutex);
		if (ft_cmp(coder->data->scheduler, "edf"))
			pthread_cond_broadcast(&coder->data->heap->h_cond);
		else
			pthread_cond_broadcast(&coder->data->queue->q_cond);
		return 0;
	}
	pthread_mutex_unlock(&coder->data->global_mutex);
	return 1;
}


int check_burnout(Coder** Coders)
{
	Data *data;
	Coder *coder;
	int j;
	long current_time;
	
	data = Coders[0]->data;
	j = 0;
	while (j < data->nb_coders)
	{
		coder = Coders[j];
		pthread_mutex_lock(&coder->c_mutex);
		current_time = gettime();
    	current_time -= data->start_time;
        if (coder->compile_count == data->nb_compiles)
        {
			j++;
			pthread_mutex_unlock(&coder->c_mutex);
			continue;
		}
    	if (current_time > (coder->last_compile + coder->data->time_to_burnout))
		{
			pthread_mutex_unlock(&coder->c_mutex);
			pthread_mutex_lock(&coder->data->global_mutex);
			coder->data->simulation_stop = 1;
			pthread_mutex_unlock(&coder->data->global_mutex);
	    	put_str_codexion(coder->index, current_time, "burned out\n");
			return 0;
	    }
		j++;
		pthread_mutex_unlock(&coder->c_mutex);
	}
	return 1;
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
	Coder **Coders;

	Coders = (Coder **)arg;
	while (!check_compiling(Coders))
	{
		if (!check_burnout(Coders))
			return NULL;
		usleep(1000);
	}

	return NULL;
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

