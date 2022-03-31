#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void swap(int *a, int *b);

// valgrind ./memory to check error
int main(void)
{
    int x = 1;
    int y = 2;

    printf("%i\t%i\n", x, y);

    swap(&x, &y);

    printf("%i\t%i\n", x, y);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
