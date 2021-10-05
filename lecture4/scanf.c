#include <stdio.h>

int main(void)
{
    int x;
    printf("x: ");
    scanf("%i", &x); // pass in address of x, so scanf can put the user's input into variable x
    printf("x: %i\n", x);

    char s[4]; // allocate 4 bytes for "HI!\0"
    printf("s: ");
    scanf("%s", s); // s is already the address since it's a char*
}