
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool contain_not_digit(string text);
void print_cipher_text(int key, string text);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string key = argv[1];
    if (contain_not_digit(key))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string text = get_string("plaintext: ");

    print_cipher_text(atoi(key), text);
}

bool contain_not_digit(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        if (!isdigit(text[i]))
        {
            return true;
        }
    }
    return false;
}

void print_cipher_text(int key, string text)
{
    printf("ciphertext: ");

    for (int i = 0; i < strlen(text); i++)
    {
        char c = toupper(text[i]);

        if (isalpha(c))
        {
            char resp = (c - 65 + key) % 26 + 65;
            if (isupper(text[i]))
            {
                printf("%c", resp);
            }
            else
            {
                printf("%c", tolower(resp));
            }
        }
        else
        {
            printf("%c", c);
        }
    }

    printf("\n");
}
