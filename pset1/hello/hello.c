#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // ask user for their name
    string answer = get_string("What is your name? ");

    // greets user with the name they provided
    printf("hello, %s\n", answer);
}