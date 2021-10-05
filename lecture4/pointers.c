#include <stdio.h>

int main(void)
{
    int n = 50;
    printf("Number is %i\n", n);
    printf("Address is %p\n", &n); // get address value of variable

    // * is deference operator - go to the location that a pointer is pointing to
    // *& means go to the address of the variable (equivalent to just calling the variable name)
    printf("Number is %i\n", *&n);

    // pointer - a variable that stores an address
    // declared with a * (pointer type)
    int *x = &n; // stores address of n into pointer x
    printf("Number is %i\n", *x); // use * to point to address
    printf("Address is %p\n", x); // display address that x contains

    char *s = "HI!";
    printf("String is %s\n", s);
}