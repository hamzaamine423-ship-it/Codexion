#include "my_header.h"

void  compiling(Coder* coder)
{
    size_t current_time;

    current_time = gettime();
    current_time -= coder->data->start_time;

	put_str_codexion(coder->index, current_time, "is compiling\n");
	usleep(coder->data->time_to_compile);

    coder->left_dongle->released = gettime();
    coder->right_dongle->released = gettime();

	coder->compile_count++;

	debugging(*coder);
}

void debugging(Coder coder)
{
    size_t current_time;

    current_time = gettime();
    current_time -= coder.data->start_time;

	put_str_codexion(coder.index, current_time, "is debugging\n");

    usleep(coder.data->time_to_debug);

    refactoring(coder);
}

void refactoring(Coder coder)
{
    size_t current_time;

    current_time = gettime();
    current_time -= coder.data->start_time;

	put_str_codexion(coder.index, current_time, "is refactoring\n");

    usleep(coder.data->time_to_refactor);
}