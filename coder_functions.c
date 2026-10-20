#include "codexion_header.h"

int	update_last_compile(t_coder *cooder, long current_time)
{
	pthread_mutex_lock(&cooder->c_mutex);
	cooder->last_compile = current_time;
	pthread_mutex_unlock(&cooder->c_mutex);
	return (0);
}

int	finish_compiling(t_coder *cooder, long *current_time)
{
	t_data	*datta;

	datta = cooder->datta;
	pthread_mutex_lock(&cooder->c_mutex);
	*current_time = gettime();
	*current_time -= datta->start_time;
	cooder->left_dongle->released = *current_time;
	cooder->right_dongle->released = *current_time;
	recovery_dongles(cooder);
	cooder->compile_count++;
	pthread_mutex_unlock(&cooder->c_mutex);
	return (0);
}

int	compiling(t_coder *cooder)
{
	long	current_time;
	t_data	*datta;

	datta = cooder->datta;
	current_time = gettime();
	current_time -= datta->start_time;
	if (!checks_simulation(cooder))
	{
		recovery_dongles(cooder);
		return (0);
	}
	update_last_compile(cooder, current_time);
	put_str_codexion(cooder->index, current_time, "is compiling\n");
	if (!ft_sleep(datta->time_to_compile, cooder, 1))
		return (0);
	if (!checks_simulation(cooder))
	{
		recovery_dongles(cooder);
		return (0);
	}
	finish_compiling(cooder, &current_time);
	return (debugging(cooder));
}

int	debugging(t_coder *cooder)
{
	long	current_time;
	t_data	*datta;

	datta = cooder->datta;
	current_time = gettime();
	current_time -= datta->start_time;
	if (!checks_simulation(cooder))
		return (0);
	put_str_codexion(cooder->index, current_time, "is debugging\n");
	if (!ft_sleep(datta->time_to_debug, cooder, 0))
		return (0);
	return (refactoring(cooder));
}

int	refactoring(t_coder *cooder)
{
	long	current_time;
	t_data	*datta;

	datta = cooder->datta;
	current_time = gettime();
	current_time -= datta->start_time;
	if (!checks_simulation(cooder))
		return (0);
	put_str_codexion(cooder->index, current_time, "is refactoring\n");
	if (!ft_sleep(datta->time_to_refactor, cooder, 0))
		return (0);
	return (1);
}
