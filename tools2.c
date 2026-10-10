#include "my_header.h"

void put_str_codexion(const char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		write(stdout, str[i], 1);
		i++;
	}
}

void unlock_dongles(Dongle* left_d, Dongle* right_d)
{
	pthread_mutex_unlock(&left_d->mutex);
	left_d->available = 1;

	pthread_mutex_unlock(&right_d->mutex);
	right_d->available = 1;
}

void compiling(Coder* coder)
{
	printf("Xms %d is compiling!\n", coder->index);
	usleep(coder->data->time_to_compile);

	unlock_dongles(coder->left_dongle, coder->right_dongle);
	coder->compile_count++;

	debugging(*coder);
}

void debugging(Coder coder)
{
    printf("Xms %d is debugging\n", coder.index);
    usleep(coder.data->time_to_debug);

    refactoring(coder);
}

void refactoring(Coder coder)
{
    printf("Xms %d is refactoring\n", coder.index);
    usleep(coder.data->time_to_refactor);
}