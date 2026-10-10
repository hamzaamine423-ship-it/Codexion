#include <stdlib.h>
#include <stdio.h>

void hamine()
{
    system("leaks leak_test");
}

int main(void)
{
    atexit(hamine);
    int *p = malloc(sizeof(int) * 100);
    if (!p)
    {
        printf("ERR\n");
        return 0;
    }
    free(p);
    int *p2 = malloc(sizeof(int) * 23);
    if (!p2){
        printf("ERR\n");
        return 0;
    }
    free(p2);
    printf("NO error!\n");
    return (0);
}