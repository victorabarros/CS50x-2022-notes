#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// typedef char *string;

int main(void)
{
    // "fancy" copy
    char *s1 = "hello!";
    char *t1 = s1; // same value, because string are pointers

    printf("%s\t%p\n", s1, s1);
    printf("%s\t%p\n\n", t1, t1);

    // "truly" copy
    char *s2 = "hello!";
    char *t2 = malloc(strlen(s2) + 1); // +1 is the \0 at the end of every string

    if (t2 == NULL)
    {
        return 1; // fail aka no memory available
    }

    strcpy(t2, s2);

    if (strlen(t2) > 0)
    {
        t2[0] = toupper(t2[0]);
    }

    printf("%s\t%p\n", s1, s1);
    printf("%s\t%p\n", t2, t2);

    free(t2);
    return 0;
}
