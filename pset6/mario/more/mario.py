import cs50


def main():
    # Ask user to prompt a heigh between 1-8
    height = get_height()

    # Print out pyramid based on given height
    generate_pyramid(height)


def get_height():
    height = 0
    while height < 1 or height > 8:
        height = cs50.get_int("Height: ")
    return height


def generate_pyramid(height):
    gap = "  "

    for row in range(1, height + 1):
        num_of_whitespaces = height - row
        num_of_hashes = row

        # Generate left side
        print(' ' * num_of_whitespaces, end='')
        print('#' * num_of_hashes, end='')

        # Insert gap
        print('  ', end='')

        # Generate right side
        print('#' * num_of_hashes, end='')

        # End of row
        print()


if __name__ == "__main__":
    main()
