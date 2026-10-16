#define _POSIX_C_SOURCE 200809L
#include "codexion_header.h"

long gettime(void)
{
    struct timespec ts;
    
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
        return (0);
    
    return ((long)(ts.tv_sec) * 1000)
                         + ((long)(ts.tv_nsec / 1000000));

}
