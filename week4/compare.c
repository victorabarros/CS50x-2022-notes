#include <stdio.h>
#include <string.h>

// typedef char *string;

int main(void)
{
    char *s = "HI!";
    char *t = "HI!";

    if (strcmp(s, t) == 0)
    {
        printf("equal\n");
    }
    else
    {
        printf("different\n");
    }

    printf("%p\n", s);
    printf("%p\n", t);
    // the compiler intended both variables to same memory, beacuse both content are equals
}
