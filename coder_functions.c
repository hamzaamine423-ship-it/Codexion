#include "codexion_header.h"


int	update_last_compile(Coder *coder, long current_time)
{
	pthread_mutex_lock(&coder->c_mutex);
	coder->last_compile = current_time;
	pthread_mutex_unlock(&coder->c_mutex);
	return (0);
}

int	finish_compiling(Coder *coder, long *current_time)
{
	Data	*data;

	data = coder->data;
	pthread_mutex_lock(&coder->c_mutex);
	*current_time = gettime();
	*current_time -= data->start_time;
	coder->left_dongle->released = *current_time;
	coder->right_dongle->released = *current_time;
	recovery_dongles(coder);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->c_mutex);
	return (0);
}

int	compiling(Coder *coder)
{
	long	current_time;
	Data	*data;

	data = coder->data;
	current_time = gettime();
	current_time -= data->start_time;
	if (!checks_simulation(coder))
	{
		recovery_dongles(coder);
		return (0);
	}
	update_last_compile(coder, current_time);
	put_str_codexion(coder->index, current_time, "is compiling\n");
	usleep(data->time_to_compile * 1000);
	if (!checks_simulation(coder))
	{
		recovery_dongles(coder);
		return (0);
	}
	finish_compiling(coder, &current_time);
	return (debugging(coder));
}


int debugging(Coder* coder)
{
    long current_time;
    Data *data;

    data = coder->data;

    current_time = gettime();
    current_time -= data->start_time;

    if (!checks_simulation(coder))
        return 0;

    put_str_codexion(coder->index, current_time, "is debugging\n");

    usleep(data->time_to_debug * 1000);

    return refactoring(coder);
}

int refactoring(Coder *coder)
{
    long current_time;
    Data* data;

    data = coder->data;
    current_time = gettime();
    current_time -= data->start_time;

    if (!checks_simulation(coder))
        return 0;

    
    put_str_codexion(coder->index, current_time, "is refactoring\n");
    
    usleep(data->time_to_refactor * 1000);
    return 1;
}
