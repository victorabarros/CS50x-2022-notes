#include <stdio.h>
#include <string.h>

typedef char *string;

int main(void)
{
    char *firstname = "Victor"; // string as pointer char
    printf("%s\n", firstname);
    printf("%p\n\n", firstname);

    for (int ii = 0; ii < strlen(firstname); ii++)
    {
        printf(
            "%c\t%c\t%p\n",
            firstname[ii],     // caracter
            *(firstname + ii), // caracter using pointer
            &firstname[ii]     // pointer
        );
    }

    string lastname = "Barros";
    printf("\n%s\n", lastname);
    printf("%p\n\n", lastname);

    for (int ii = 0; ii < strlen(lastname); ii++)
    {
        printf("%c\t%p\n", lastname[ii], &lastname[ii]);
    }

    printf("\nnumbers\n");
    int numbers[] = {4, -9, 10, 2};
    // array can be treated as the address to the first element
    size_t numbersLength = sizeof(numbers) / sizeof(numbers[0]);

    for (int ii = 0; ii < numbersLength; ii++)
    {
        printf(
            "%i\t%i\t%p\n",
            *(numbers + ii),
            numbers[ii],
            &numbers[ii]
            // incresing 4 because is the size of each integer, in bytes. in other hand char is only one byte
        );
    }
}
