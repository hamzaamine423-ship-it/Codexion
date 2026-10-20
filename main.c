#include "codexion_header.h"

t_monitor	*start_monitor(t_coder **coders, int burnout, t_data *datta)
{
	t_monitor	*moonitor;

	moonitor = malloc(sizeof(t_monitor));
	if (!moonitor)
		free_and_exit(datta);
	moonitor->burnout = burnout;
	pthread_create(&moonitor->thread, NULL, &monitor_check, coders);
	return (moonitor);
}

void	start_function(t_data *datta)
{
	t_heap	*heapp;
	t_queue	*queuee;

	if (ft_cmp(datta->scheduler, "edf"))
	{
		heapp = init_heap(*datta);
		if (!heapp)
			free_and_exit(datta);
		datta->heapp = heapp;
		start_edf(datta->coders, *datta);
	}
	else
	{
		queuee = init_queue(*datta);
		if (!queuee)
			free_and_exit(datta);
		datta->queuee = queuee;
		start_fifo(datta->coders, *datta);
	}
}

int	main(int ac, char *av[])
{
	int			*list;
	t_data		datta;
	t_coder		**coders;
	t_monitor	*moonitor;

	list = parse(ac, av);
	if (!list)
		exit(0);
	datta = create_data(&list, av[8]);
	coders = creating_coders(&datta);
	if (!coders)
		exit(0);
	datta.coders = coders;
	datta.start_time = gettime();
	moonitor = start_monitor(coders, datta.time_to_burnout, &datta);
	datta.moonitor = moonitor;
	start_function(&datta);
	pthread_join(moonitor->thread, NULL);
	free_and_exit(&datta);
	return (0);
}
