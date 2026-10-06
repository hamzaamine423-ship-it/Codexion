#include "my_header.h"

void*	compiling(void* arg){
	

}


int main(int ac, char* av[])
{
	int *list;
	Data data;
	Coder **Coders;
	Dongle **Dongles;

	list = parse(ac, av);
	if (!list)
		exit(0);

	data = create_data(list, av[8]);
	Coders = creating_coders(data, Dongles, list);
	Dongles = creating_dongles(data, Coders, list);

	free_and_exit(Dongles, Coders, data, list);
	return 0;
}
