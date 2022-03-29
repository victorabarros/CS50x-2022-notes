#include <stdio.h>
#include <string.h>

typedef char *string;

int main(void) {
    char *firstname = "Victor";
    printf("%s\n", firstname);
    printf("%p\n\n", firstname);

    for(int ii = 0; ii < strlen(firstname); ii++) {
        printf("%c\t%p\n", firstname[ii], &firstname[ii]);
    }

    string lastname = "Barros";
    printf("\n%s\n", lastname);
    printf("%p\n\n", lastname);

    for(int ii = 0; ii < strlen(lastname); ii++) {
        printf("%c\t%p\n", lastname[ii], &lastname[ii]);
    }

}
