#include <cs50.h>
#include <stdio.h>
#include <string.h>

// takes integer and an array of strings
// argc = argument count; number of strings that the human typed
// argv = argument vector; all of the strings that the human typed
// argv[0] is always the program name (i.e. "./argv")
int main(int argc, string argv[])
{
    if (argc == 2)
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
        {
            printf("%c\n", argv[1][i]);
        }
    }
    else
    {
        printf("hello, world\n");
    }
}