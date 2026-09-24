#include <stdio.h>
#include <stdlib.h>


int len(int *list)
{
    return sizeof(list) / sizeof(list[0]);
}



int main()
{
    int list[] = {10, 20, 0, 40, 50};
    int le = len(list);
    printf("%d", le);

}