// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];
int tableSize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    for (node *tmp = table[hash(word)]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Ref https://youtu.be/nvzVHwrrub0?t=290
    int sum = 0;
    for (int ii = 0; word[ii] != '\0'; ii++)
    {
        sum += tolower(word[ii]);
    }
    return sum % N;
    // return toupper(word[0]) - 'A'; // I think this one is more elegant. but doesn't work to N != 26 and larger N make check faster
}

// Loads dictionary into memory (hash table = array of linked list), returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(input, "%s", word) != EOF)
    {
        node *newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            printf("Could not allocate memory.\n");
            return false;
        }

        int idx = hash(word);

        strcpy(newNode->word, word);
        newNode->next = table[idx];
        table[idx] = newNode;

        tableSize++;
    }
    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return tableSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int idx = 0; idx < N; idx++)
    {
        while (table[idx] != NULL)
        {
            node *tmp = table[idx]->next;
            free(table[idx]);
            table[idx] = tmp;
        }
    }
    return true;
}
