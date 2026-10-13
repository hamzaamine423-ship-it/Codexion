#include "my_header.h"

void  compiling(Coder* coder)
{
    size_t current_time;
    Data *data;

    data = coder->data;
    current_time = gettime();
    current_time -= coder->data->start_time;

    pthread_mutex_lock(&data->global_mutex);
	put_str_codexion(coder->index, current_time, "is compiling\n");
	usleep(coder->data->time_to_compile * 1000);

    current_time = gettime();
    current_time -= coder->data->start_time;
    coder->last_compile = current_time;
    coder->left_dongle->released = current_time;
    coder->right_dongle->released = current_time;
    unlock_dongles(coder->left_dongle, coder->right_dongle);
	coder->compile_count++;
    pthread_mutex_unlock(&data->global_mutex);


	debugging(coder);
}

void debugging(Coder *coder)
{
    size_t current_time;

    current_time = gettime();
    current_time -= coder->data->start_time;

    // write(1, "I AM WAITING\n", 13);
    pthread_mutex_lock(&coder->data->global_mutex);
    // write(1, "Perfect\n", 8);
	put_str_codexion(coder->index, current_time, "is debugging\n");

    usleep(coder->data->time_to_debug * 1000);
    pthread_mutex_unlock(&coder->data->global_mutex);

    refactoring(coder);
}

void refactoring(Coder *coder)
{
    size_t current_time;

    current_time = gettime();
    current_time -= coder->data->start_time;

    pthread_mutex_lock(&coder->data->global_mutex);
	put_str_codexion(coder->index, current_time, "is refactoring\n");

    usleep(coder->data->time_to_refactor * 1000);
    pthread_mutex_unlock(&coder->data->global_mutex);
}