// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 4000; // each bucket represents an alphabet (i.e. 0 = 'A' and 25 = 'Z')

// GLOBAL VARIABLES
node *table[N]; // Hash table
int dict_size = 0; // size of dictionary

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_val = hash(word); // hash word to obstain a hash value

    // traverse linked list, looking for the word
    node *cursor = table[hash_val]; // access linked list at that index in the hash table
    while (cursor != NULL) // check only if node exist
    {
        // check if word match (case-insensitive)
        if (strcasecmp(word, cursor->word) == 0) // 0 == match
        {
            return true;
        }

        // check next node in linked list
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
// Hash function produced by Professor Daniel J. Bernstein (DJB Hash Function)
// Credit to CS50 user "Minh Cửu Nguyễn" for sharing the hash function. URL Source - https://edstem.org/us/courses/176/discussion/358623
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    // output needs to be between 0 and N-1 (0-25)
    hash %= N; // get appropriate range to fit bucket
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char word_from_dict[LENGTH];

    // read strings from file one word at a time until EOF
    while (fscanf(dict, "%s", word_from_dict) != EOF)
    {
        node *n = malloc(sizeof(node)); // allocate memory for new node
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word_from_dict); // put string into node
        n->next = NULL; // initialize pointer

        int hash_val = hash(word_from_dict); // get hash value (0-25)

        // store in hash table
        n->next = table[hash_val]; // point new node where bucket is pointing at
        table[hash_val] = n; // point bucket at new node

        dict_size++; // keep track of words in the dictionary
    }

    // close dictionary file
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // if table is not loaded or no words in dictionary file, then dict_size will be 0
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // iterate through table[]
    for (int i = 0; i < N; i++)
    {
        // free any nodes in the linked list
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor; // store current node
            cursor = cursor->next; // point cursor to next pointer
            free(tmp); // free current node
        }
    }

    return true;
}
