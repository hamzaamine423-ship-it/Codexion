#define _POSIX_C_SOURCE 200809L
#include "my_header.h"
#include <time.h>

size_t gettime(void)
{
    struct timespec ts;
    
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
        return (0);
    
    return ((size_t)(ts.tv_sec) * 1000)
                         + ((size_t)(ts.tv_nsec / 1000000));

}
