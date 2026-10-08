#include "my_header.h"


void unlock_dongles(Dongle* left_d, Dongle* right_d)
{
	pthread_mutex_unlock(&left_d->mutex);
	left_d->available = 1;

	pthread_mutex_unlock(&right_d->mutex);
	right_d->available = 1;
}

int check_dongles_availability(Coder *coder)
{
	Dongle* left_d;
	Dongle* right_d;

	left_d = coder->left_dongle;
	right_d = coder->right_dongle;

	if (right_d->available && left_d->available)
	{
		printf("the right and left dongles are free to use !!\n");
		fflush(stdout);
		return 1;
	}

	else
		return 0;
}

void push_heap(Heap* heap, Coder* new_coder)
{
	int child_i;
	int parent_i;
	Coder* parent;
	Coder* child;
	int debug;


	if (!heap->size)
	{
		heap->Coders[0] = new_coder;
		heap->size++;
		return;
	}
	if (heap->size == 3)
		debug = 1;
	else
		debug = 0;
	
	heap->Coders[heap->size] = new_coder;
	heap->size++;

	

	// printf("index of the new coder: %d\n", new_coder->index);
	// printf("size: %d\n", heap->size);

	child_i = heap->size;
	while (1)
	{
		if (child_i == 0)
			return;
		parent_i = child_i / 2;
		// printf("parent_i=%d, child_i=%d\n", parent_i, child_i);
		parent = heap->Coders[parent_i - 1];


		child = heap->Coders[child_i - 1];
		
		if (parent->index > child->index)
		{
			// printf("index of parent before: %d\n", heap->Coders[parent_i - 1]->index);
			heap->Coders[parent_i - 1] = child;
			heap->Coders[child_i - 1] = parent;
			child_i = parent_i / 2;
			// printf("index of parent after: %d\n", heap->Coders[parent_i - 1]->index);
		}
		else
			break;
	}

}

Coder* pop_heap(Heap* heap)
{
	Coder* top_coder;

	if (heap->size == 0)
		return NULL;
	top_coder = heap->Coders[0];

	remove_top_heap(heap);

	return top_coder;
}

void* compiling(void* arg) 
{
	Coder *coder;
	int ret;

	coder = (Coder *)arg;
	printf("Xms %d is compiling!\n", coder->index);
	usleep(coder->data.time_to_compile);

	unlock_dongles(coder->left_dongle, coder->right_dongle);
	coder->compile_count++;
	debugging(*coder);

	return NULL;
}

void taking_dongles(Heap* heap)
{
	Coder *coder1;

	coder1 = heap->Coders[0];
	if(check_dongles_availability(coder1))
	{
		printf("Xms %d has taken a dongle\n", coder1->index);
		printf("Xms %d has taken a dongle\n", coder1->index);
		if (pthread_create(&coder1->thread, NULL, &compiling, coder1))
			perror("Failed to create a thread !!\n");
	}
}


int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Heap *heap;
	Coder *tmp;

	printf("\n");
	list = parse(ac, av);
	if (!list)
		exit(0);

	data = create_data(list, av[8]);
	Coders = creating_coders(data, list);


	heap = create_heap_FIFO(data);
	if (!heap)
		free_and_exit(Coders, data, list);




	for (int i = 9; i > 6; i--){
		push_heap(heap, Coders[i]);
		printf("size of the heap: %d\n", heap->size);
	}

	printf("size of heap: %d\n\n", heap->size);

	for(int i=0; i < heap->size; i++){
		printf("heap[%d]: %d\n",i + 1, heap->Coders[i]->index);
	}

	// for(int i = 0; i < data.nb_coders; i++){
	// 	tmp = heap->Coders[i];
	// 	pthread_join(tmp->thread, NULL);
	// }

	free_and_exit(Coders, data, list);
	return 0;
}
