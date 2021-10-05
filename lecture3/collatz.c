#include <cs50.h>
#include <stdio.h>

int collatz(int);

int main(void)
{
    int input = get_int("Enter n: ");
    printf("%i steps\n", collatz(input));
}

int collatz(int n)
{
    printf("n = %i\n", n);

    // base case
    if (n == 0 || n == 1)
    {
        return 0;
    }
    // even number
    else if (n % 2 == 0)
    {
        return 1 + collatz(n/2);
    }
    // odd numbers
    else
    {
        return 1 + collatz(3 * n + 1);
    }
}