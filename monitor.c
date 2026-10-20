#include "codexion_header.h"

int	checks_simulation(t_coder *cooder)
{
	pthread_mutex_lock(&cooder->datta->global_mutex);
	if (cooder->datta->simulation_stop == 1)
	{
		pthread_mutex_unlock(&cooder->datta->global_mutex);
		return (0);
	}
	pthread_mutex_unlock(&cooder->datta->global_mutex);
	return (1);
}

int	check_burnout2(t_coder *cooder, long current_time)
{
	if (current_time > (cooder->last_compile + cooder->datta->time_to_burnout) + 10)
	{
		pthread_mutex_unlock(&cooder->c_mutex);
		pthread_mutex_lock(&cooder->datta->global_mutex);
		cooder->datta->simulation_stop = 1;
		pthread_mutex_unlock(&cooder->datta->global_mutex);
		put_str_codexion(cooder->index, current_time, "burned out\n");
		return (0);
	}
	return (1);
}

int	check_burnout(t_coder **coders)
{
	t_coder	*cooder;
	int		j;
	long	current_time;

	j = 0;
	while (j < coders[0]->datta->nb_coders)
	{
		cooder = coders[j];
		pthread_mutex_lock(&cooder->c_mutex);
		current_time = gettime();
		current_time -= cooder->datta->start_time;
		if (cooder->compile_count == cooder->datta->nb_compiles)
		{
			j++;
			pthread_mutex_unlock(&cooder->c_mutex);
			continue ;
		}
		if (!check_burnout2(cooder, current_time))
			return (0);
		j++;
		pthread_mutex_unlock(&cooder->c_mutex);
	}
	return (1);
}

int	check_compiling(t_coder **coders)
{
	int		compile_count;
	t_data	*datta;
	t_coder	*cooder;
	int		i;

	datta = coders[0]->datta;
	compile_count = datta->nb_compiles;
	i = 0;
	while (i < datta->nb_coders)
	{
		cooder = coders[i];
		pthread_mutex_lock(&cooder->c_mutex);
		if (compile_count != cooder->compile_count)
		{
			pthread_mutex_unlock(&cooder->c_mutex);
			return (0);
		}
		pthread_mutex_unlock(&cooder->c_mutex);
		i++;
	}
	return (1);
}

void	*monitor_check(void *arg)
{
	t_coder	**coders;

	coders = (t_coder **)arg;
	while (!check_compiling(coders))
	{
		if (!check_burnout(coders))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
