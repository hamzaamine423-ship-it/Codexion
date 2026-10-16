#include "codexion_header.h"


int compiling(Coder* coder)
{
    long current_time;
    Data* data;

    data = coder->data;
    current_time = gettime();
    current_time -= data->start_time;

    pthread_mutex_lock(&coder->data->global_mutex);
    if (!checks_simulation(coder))
        return 0;
    coder->last_compile = current_time;
    pthread_mutex_unlock(&coder->data->global_mutex);
    put_str_codexion(coder->index, current_time, "is compiling\n");
    usleep(data->time_to_compile * 1000);
    pthread_mutex_lock(&coder->data->global_mutex);
    if (!checks_simulation(coder))
        return 0;
    current_time = gettime();
    current_time -= data->start_time;

    coder->left_dongle->released = current_time;
    coder->right_dongle->released = current_time;
    recovery_dongles(coder);
    coder->compile_count++;
    pthread_mutex_unlock(&coder->data->global_mutex);

    return debugging(coder);
}


int debugging(Coder* coder)
{
    long current_time;
    Data *data;

    data = coder->data;

    current_time = gettime();
    current_time -= data->start_time;

    pthread_mutex_lock(&coder->data->global_mutex);
    if (!checks_simulation(coder))
        return 0;    
    put_str_codexion(coder->index, current_time, "is debugging\n");
    pthread_mutex_unlock(&coder->data->global_mutex);

    usleep(data->time_to_debug * 1000);

    return refactoring(coder);
}

int refactoring(Coder *coder)
{
    long current_time;
    Data* data;

    data = coder->data;
    current_time = gettime();
    current_time -= data->start_time;

    pthread_mutex_lock(&coder->data->global_mutex);
    if (!checks_simulation(coder))
        return 0;  
    put_str_codexion(coder->index, current_time, "is refactoring\n");
    pthread_mutex_unlock(&coder->data->global_mutex);
    usleep(data->time_to_refactor * 1000);
    return 1;
}
