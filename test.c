#include <stdio.h>

void swap(int *a, int *b)
{// YOUR CODE HERE

	char x;
	int t;
	t = *a;
	*a = *b;
	*b = t;

}


int main(void)
{
    int x = 10;
    int y = 20;

    printf("before: x = %d, y = %d\n", x, y);

    swap(&x, &y);

    printf("after:  x = %d, y = %d\n", x, y);

    return 0;
}