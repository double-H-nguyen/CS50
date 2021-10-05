#include <cs50.h>
#include <stdio.h>


int get_height(void);
void generate_pyramid(int);
void print_character(char, int);


int main(void)
{
    // ask user for height
    int height = get_height();

    // print out pyramid based on given height
    generate_pyramid(height);
}


int get_height(void)
{
    int answer = 0;

    while (answer < 1 || answer > 8)
    {
        answer = get_int("Enter a value between 1-8: ");
    }

    return answer;
}


void generate_pyramid(int height)
{
    // start at 1 since we normally start at row #1
    for (int row = 1; row <= height; row++)
    {
        // calculate how many whitespaces and hashes for each row
        // whitespace should fill in areas not occupied by row (excluding gap)
        // there should be as many hashes as the row number
        int num_of_whitespaces = height - row;
        int num_of_hashes = row;

        // generate left side
        print_character(' ', num_of_whitespaces);
        print_character('#', num_of_hashes);

        // insert gap
        printf("  ");

        // generate right side
        print_character('#', num_of_hashes);

        // end of row
        printf("\n");
    }
}


void print_character(char character, int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("%c", character);
    }
}