#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // initialize file type, then open phonebook file in append mode
    FILE *file = fopen("phonebook.csv", "a");

    // error checking
    if (file == NULL)
    {
        return 1;
    }

    char *name = get_string("Name: ");
    char *number = get_string("Number: ");

    // print to file
    fprintf(file, "%s,%s\n", name, number);

    // close file
    fclose(file);
}