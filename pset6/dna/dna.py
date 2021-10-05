import sys
import csv


def main():
    # Verify command-line arguments
    if len(sys.argv) != 3:
        print("Usage: python dna.py databases/__.csv sequences/#.txt")

    # Open csv file
    people = read_database(sys.argv[1])

    # Open DNA sequence file
    sequence = read_sequence(sys.argv[2])

    # Check list of people for a match
    # If match, print name and exit
    for person in people:
        name = person["name"]
        if is_match(person, sequence):
            print(name)
            sys.exit(0)

    # If no match, print no match and exit
    print("No match")
    sys.exit(1)


def read_database(filename):
    people = []

    with open(filename, "r") as file:
        reader = csv.DictReader(file)

        # Add each individual in csv to list in dictionary format
        for row in reader:
            # Make sure numbers are integers
            for key, value in row.items():
                if key != "name":
                    row[key] = int(value)
            people.append(row)

    return people


def read_sequence(filename):
    with open(filename, "r") as file:
        # Return sequence read from file
        return file.read()


def is_match(person, sequence):
    # Check if the consecutive STR count matches the person's value
    for key, value in person.items():
        # key=STR, value=count
        if key != "name":
            max_repeat = get_max_repeats(sequence, key)
            # Return false if no match
            if max_repeat != value:
                return False
    # Return true if all STR counts matches
    return True


def get_max_repeats(sequence, STR):
    STR_length = len(STR)
    max_repeat = 0

    # Search for STR in sequence
    for i in range(0, len(sequence)):
        STR_repeat = 0
        # If match is found, keep searching for consecutive matches
        while STR == sequence[i:i + STR_length]:
            STR_repeat += 1
            # Search for next consecutive match
            i += STR_length

        # Update max value
        if STR_repeat > max_repeat:
            max_repeat = STR_repeat

    return max_repeat


if __name__ == "__main__":
    main()
