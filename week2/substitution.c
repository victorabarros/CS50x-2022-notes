#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void print_cipher_text(string key, string text);
char cipher_char(string key, char c);
bool contain_not_alphabetic(string key);
bool contain_char_repeated(string text);

int main(int argc, string argv[])
{
    // key exists
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // key length
    string key = argv[1];
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // key is alphabetic
    if (contain_not_alphabetic(key))
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }

    // key repeat it self
    if (contain_char_repeated(key))
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    string text = get_string("plaintext: ");

    print_cipher_text(key, text);
}

bool contain_not_alphabetic(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        if (!isalpha(text[i]))
        {
            return true;
        }
    }
    return false;
}

bool contain_char_repeated(string text)
{
    int key_char_qnt[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < strlen(text); i++)
    {
        int idx = toupper(text[i]) - 65;

        key_char_qnt[idx]++;
        if (key_char_qnt[idx] > 1)
        {
            return true;
        }
    }

    return false;
}

void print_cipher_text(string key, string text)
{
    printf("ciphertext: ");

    for (int i = 0; i < strlen(text); i++)
    {
        printf("%c", cipher_char(key, text[i]));
    }

    printf("\n");
}

char cipher_char(string key, char c)
{
    if (!isalpha(c))
    {
        return c;
    }

    int index = toupper(c) - 65;
    char cipher_char = key[index];

    if (isupper(c))
    {
        return toupper(cipher_char);
    }
    else
    {
        return tolower(cipher_char);
    }
}
