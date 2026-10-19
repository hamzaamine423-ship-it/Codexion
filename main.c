#include "codexion_header.h"


int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Heap *heap;
	Queue *queue;
	Monitor *monitor;

	list = parse(ac, av);
	if (!list)
		exit(0);

	data = create_data(&list, av[8]);

	Coders = creating_coders(&data);
	if (!Coders)
		exit(0);

	data.Coders = Coders;

	heap = init_heap(data);
	if (!heap)
		free_and_exit(Coders, NULL);

	queue = init_queue(data);
	if (!queue)
		free_and_exit(Coders, NULL);

	data.heap = heap;
	data.queue = queue;

	data.start_time = gettime();
	monitor = start_monitor(Coders, data.time_to_burnout);

	if (ft_cmp(data.scheduler, "edf"))
		start_edf(Coders, data);
	else
		start_fifo(Coders, data);
	
	pthread_join(monitor->thread, NULL);

	free_and_exit(Coders, &heap);

	return 0;
}



