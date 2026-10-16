#include "codexion_header.h"


void compiling(Coder* coder)
{
    long current_time;
    Data* data;

    data = coder->data;
    current_time = gettime();
    current_time -= data->start_time;

    put_str_codexion(coder->index, current_time, "is compiling\n");
    usleep(data->time_to_compile * 1000);
    if (pthread_mutex_lock(&coder->data->global_mutex) != 0)
        exit(1);
    current_time = gettime();
    current_time -= data->start_time;
    coder->last_compile = current_time;

    coder->left_dongle->released = current_time;
    coder->right_dongle->released = current_time;
    recovery_dongles(coder);
    coder->compile_count++;
    if (pthread_mutex_unlock(&coder->data->global_mutex) != 0)
        exit(1);

    debugging(coder);
}


void debugging(Coder* coder)
{
    long current_time;
    Data *data;

    data = coder->data;

    current_time = gettime();
    current_time -= data->start_time;

    put_str_codexion(coder->index, current_time, "is debugging\n");
    usleep(data->time_to_debug * 1000);

    refactoring(coder);
}

void refactoring(Coder *coder)
{
    long current_time;
    Data* data;

    data = coder->data;
    current_time = gettime();
    current_time -= data->start_time;

    put_str_codexion(coder->index, current_time, "is refactoring\n");
    usleep(data->time_to_refactor * 1000);
}