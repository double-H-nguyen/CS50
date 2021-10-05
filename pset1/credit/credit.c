#include <cs50.h>
#include <stdio.h>
#include <math.h>


bool valid_checksum(long);
void print_card_brand(long);
int get_digit_length(long);
int get_digit_value(long, int);


int main(void)
{
    // ask for user's credit card number
    long credit_card_num = get_long("Please enter a valid credit card number with no dashes: ");

    // use Luhn's Algorithm to verify card's checksum value
    if (valid_checksum(credit_card_num))
    {
        print_card_brand(credit_card_num);
    }
    else
    {
        printf("INVALID\n");
    }
}


bool valid_checksum(long num)
{
    int length = get_digit_length(num);

    int odd_digit_sum = 0;
    int even_digit_sum = 0;
    int total = 0;

    // Luhn’s Algorithm step 1
    // far-right value represents i = 1
    for (int i = 1; i <= length; i++)
    {
        // check if digit place is even or odd
        if (i % 2 == 0)
        {
            long temp_num = get_digit_value(num, i);
            temp_num *= 2;

            // if value > 9 (2-digit number), then sum tens place and ones place values
            if (temp_num > 9)
            {
                temp_num = get_digit_value(temp_num, 2) + get_digit_value(temp_num, 1);
            }

            // add to even digit sum
            even_digit_sum += (int)temp_num;
        }
        else // if odd digit place, add to odd_digit_sum
        {
            odd_digit_sum += get_digit_value(num, i);
        }
    }

    // Luhn’s Algorithm step 2
    total = even_digit_sum + odd_digit_sum;

    // Luhn’s Algorithm step 3
    // if total sum's ones place value is 0, then card is valid
    return (total % 10) == 0;
}


void print_card_brand(long num)
{
    int length = get_digit_length(num);
    int num_15th = get_digit_value(num, length);
    int num_14th = get_digit_value(num, length - 1);

    // AMEX uses 15 digits and starts with 34 or 37
    if (length == 15 && num_15th == 3 && (num_14th == 4 || num_14th == 7))
    {
        printf("AMEX\n");
    }
    // VISA uses 13 digits and starts with 4
    else if (length == 13 && num_15th == 4)
    {
        printf("VISA\n");
    }
    // VISA uses 16 digits and starts with 4
    // MASTERCARD uses 16 digits and starts with 51,52,53,54,55
    else if (length == 16)
    {
        if (num_15th == 4)
        {
            printf("VISA\n");
        }
        else if (num_15th == 5 && num_14th >= 1 && num_14th <= 5)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


int get_digit_length(long num)
{
    int length = 0;
    long result = num;

    // divide the number by 10 to determine length
    // the length of the digit is found when the division result equals to 0
    while (result > 0)
    {
        result /= 10;
        length++;
    }

    return length;
}


// extract value at a specific digit place from a number
// the 1st digit place represents the ones place
int get_digit_value(long org_num, int digit_place)
{
    long value = org_num;

    // if desired digit_place is greater than the ones place, continue
    if (digit_place != 1)
    {
        // reduce number until the specified digit place is in the ones place
        for (int i = 1; i < digit_place; i++)
        {
            value /= 10;
        }
    }

    // extract value from one-digit place
    value %= 10;

    return value;
}