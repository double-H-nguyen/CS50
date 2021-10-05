#include <cs50.h>
#include <stdio.h>

// Prototypes
int get_initial_population(void);
int get_end_population(int);


int main(void)
{
    // TODO: Prompt for start size
    int initial_population = get_initial_population();

    // TODO: Prompt for end size
    int ending_population = get_end_population(initial_population);

    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    int population = initial_population;
    // keep calculating until ending population goal is reached
    while (population < ending_population)
    {
        year++;

        int born = population / 3;
        int died = population / 4;
        // updating population numbers
        population += (born - died);
    }

    // TODO: Print number of years
    printf("Years: %i\n", year);
}


int get_initial_population(void)
{
    int initial_population;

    do
    {
        initial_population = get_int("Enter a starting population size: ");

        // starting population must be greater than or equal to 9 to avoid stagnation
        if (initial_population < 9)
        {
            printf("Please enter a starting population size greater than or equal to 9\n");
        }
    }
    while (initial_population < 9);

    return initial_population;
}


int get_end_population(int initial_population)
{
    int ending_population;

    do
    {
        ending_population = get_int("Enter an ending population size: ");

        // ending population must be greater than starting population
        if (ending_population < initial_population)
        {
            printf("Please enter an ending population size greater than or equal to the starting population size (%i)\n", initial_population);
        }
    }
    while (ending_population < initial_population);

    return ending_population;
}