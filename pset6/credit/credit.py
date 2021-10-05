import cs50


def main():
    # Ask for user's credit card number
    credit_card_num = cs50.get_int("Please enter a valid credit card number with no dashes: ")

    # Use Luhn's Algorithm to verify card's checksum value
    if valid_checksum(credit_card_num):
        print_card_brand(credit_card_num)
    else:
        print("INVALID")


def valid_checksum(num):
    length = len(str(num))

    odd_digit_sum = 0
    even_digit_sum = 0
    total = 0

    # Luhn’s Algorithm step 1
    # Far-right value represents i = 1
    for i in range(1, length + 1):
        # Check if digit place is even or odd
        if i % 2 == 0:
            temp_num = get_digit_value(num, i)
            temp_num *= 2

            # If value > 9 (2-digit number), then sum tens place and ones place values
            if temp_num > 9:
                temp_num = get_digit_value(temp_num, 2) + get_digit_value(temp_num, 1)

            # Add to even digit sum
            even_digit_sum += temp_num
        else:
            odd_digit_sum += get_digit_value(num, i)

    # Luhn’s Algorithm step 2
    total = even_digit_sum + odd_digit_sum

    # Luhn’s Algorithm step 3
    # If total sum's ones place value is 0, then card is valid
    return (total % 10) == 0


def print_card_brand(num):
    length = len(str(num))
    num_15th = get_digit_value(num, length)
    num_14th = get_digit_value(num, length - 1)

    # AMEX uses 15 digits and starts with 34 or 37
    if length == 15 and num_15th == 3 and (num_14th == 4 or num_14th == 7):
        print("AMEX")
    elif length == 13 and num_15th == 4:
        print("VISA")
    # VISA uses 16 digits and starts with 4
    # MASTERCARD uses 16 digits and starts with 51,52,53,54,55
    elif length == 16:
        if num_15th == 4:
            print("VISA")
        elif num_15th == 5 and 1 <= num_14th <= 5:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


# Extract value at a specific digit place from a number
# The 1st digit place represents the ones place
def get_digit_value(org_num, digit_place):
    # Convert credit card number to string
    temp_str = str(org_num)

    # Extract digit by treating the string like an array and use reverse indexing
    # i.e. for "54321", you can get "1" at [-1], and "5" at [-5]
    value = temp_str[digit_place * -1]

    return int(value)


if __name__ == "__main__":
    main()
