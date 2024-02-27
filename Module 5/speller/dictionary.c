// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[26];

// Declare variables (she've made this on her video idk why yet)
unsigned int word_count;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int bucket = hash(&word[0]);
    for(node *i = table[bucket]; i != NULL; i = i->next)
    {
        if(strcasecmp(word, i->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    char tmp = tolower(*word);
    unsigned int index = tmp - 97;

    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    for (int i = 0; i < (N - 1); i++)
    {
        table[i] = NULL;
    }

    FILE *dic = fopen(dictionary, "r");
    if(dic == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while(fscanf(dic, "%s", word) != EOF)
    {
        node *tmp = malloc(sizeof(node));

        if(tmp == NULL)
        {
            return false;
        }

        strcpy(tmp->word, word);
        unsigned int index = hash(&word[0]);
        tmp->next = table[index];
        table[index] = tmp;
        word_count++;
    }
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if(word_count > 0)
    {
        return word_count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for(int i = 0; i < N; i++)
    {
        node *word = table[i];
        while(word != NULL)
        {
            node *tmp = word;
            word = word->next;
            free(tmp);
        }
    }
    return true;
}
