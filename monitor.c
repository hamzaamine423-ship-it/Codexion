#include "codexion_header.h"


int checks_simulation(Coder *coder)
{
	if (coder->data->simulation_stop == 1)
	{
		pthread_mutex_unlock(&coder->data->global_mutex);
		return 0;
	}
	return 1;
}


int check_burnout(Coder** Coders)
{
	Data *data;
	int j;
	long current_time;
	
	data = Coders[0]->data;
	j = 0;
	while (j < data->nb_coders)
	{

		current_time = gettime();
    	current_time -= data->start_time;

        if (Coders[j]->compile_count == data->nb_compiles)
        {
			j++;
			continue;
		}
    	if (current_time > (Coders[j]->last_compile + Coders[j]->data->time_to_burnout))
		{
			Coders[j]->data->simulation_stop = 1;
	    	put_str_codexion(Coders[j]->index, current_time, "burned out\n");
			return 0;
	    }
		j++;
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
			break;
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

