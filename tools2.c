#include "my_header.h"



void debugging(Coder coder)
{
    printf("Xms %d is debugging\n", coder.index);
    usleep(coder.data.time_to_debug);
    refactoring(coder);
}

void refactoring(Coder coder)
{
    printf("Xms %d is refactoring\n", coder.index);
    usleep(coder.data.time_to_refactor);
}