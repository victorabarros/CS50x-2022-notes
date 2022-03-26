#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // for each line
    for (int i = 0; i < n; i++)
    {
        // for each column/character to printing first pyramid
        for (int j = 0; j < n; j++)
        {
            // validate what print, space or #
            if (j < n - i - 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        printf("\n");
    }
}
