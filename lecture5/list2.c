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
    node *list = NULL;

    // create first node
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        return 1;
    }
    n->number = 1;
    n->next = NULL;

    list = n; // connect first node to list

    // create second node
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(list); // if something bad happens, free first node before exiting
        return 1;
    }
    n->number = 2;
    n->next = NULL;
    list->next = n; // point first node to second node

    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(list->next); // free second node
        free(list); // free first node
        return 1;
    }
    n->number = 3;
    n->next = NULL;
    list->next->next = n;

    // iterate through list, print value in each node
    // point at beginning of list, keep going until pointer is NULL, point to next node location
    for (node *tmp = list; tmp != NULL; tmp = tmp->next)
    {
        printf("%i\n", tmp->number);
    }

    // free entire list
    while (list != NULL)
    {

        node *tmp = list->next; // free the next node instead of the first node, so you don't leave the nodes stranded with no way to free them
        free(list); // free first node
        list = tmp; // now make the next node the first node, and repeat
    }
    free(list)
    return 0;
}