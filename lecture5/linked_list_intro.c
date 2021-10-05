#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int number;
    struct node *next;
}
node;

int main(void)
{
    node *list = NULL; // create list

    // create first node and assign its number to 1
    node *n = malloc(sizeof(node)); // create node and provide space for it in memory
    if (n != NULL) // always check if a pointer is not NULL before doing something with it
    {
        // go to address of node, and assign 1 to the number
        // "n->"" is equivalent to "(*n).""
        // (*n).number = 1;
        n->number = 1;
        n->next = NULL;
        list = n; // list will now point to the first node
    }

    // create second node and assign its number to 2
    node *n2 = malloc(sizeof(node));
    if (n2 != NULL)
    {
        n2->number = 2;
        n2->next = NULL;
        list->next = n2; // point first node to second node
    }

    // create third node and assign its number to 3
    node *n3 = malloc(sizeof(node));
    if (n3 != NULL)
    {
        n3->number = 3;
        n3->next = NULL;
        list->next->next = n3;
    }
}