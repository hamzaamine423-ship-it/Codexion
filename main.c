#include "codexion_header.h"

void start_function(Data *data)
{
	Heap *heap;
	Queue *queue;

	if (ft_cmp(data->scheduler, "edf"))
	{
		heap = init_heap(*data);
		if (!heap)
			free_and_exit(data);
		data->heap = heap;

		start_edf(data->Coders, *data);
	}
	else
	{
		queue = init_queue(*data);
		if (!queue)
			free_and_exit(data);

		data->queue = queue;
		start_fifo(data->Coders, *data);
	}
}

void hamine(void)
{
	system("leaks codexion");
}


int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Monitor *monitor;

	// atexit(hamine);
	list = parse(ac, av);
	if (!list)
		exit(0);
	data = create_data(&list, av[8]);
	Coders = creating_coders(&data);
	if (!Coders)
		exit(0);

	data.Coders = Coders;
	data.start_time = gettime();
	monitor = start_monitor(Coders, data.time_to_burnout, &data);
	data.monitor = monitor;
	start_function(&data);

	pthread_join(monitor->thread, NULL);
	free_and_exit(&data);

	return 0;
}



