#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


bool check_non_alphabetic(string, int);
bool check_duplicate(string, int);
string substitution_cipher(string, string);


int main(int argc, string argv[])
{
    // argv[0] is the program itself
    // check if user provided key
    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    int key_length = strlen(key);

    // check if user provided a key of 26 characters
    if (key_length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check if key does contains non-alphabetic character
    if (check_non_alphabetic(key, key_length))
    {
        printf("Key cannot contain non-alphabetical characters.\n");
        return 1;
    }

    // check for duplicate letters
    if (check_duplicate(key, key_length))
    {
        printf("Key cannot contain duplicate letters.\n");
        return 1;
    }

    // prompt user for plaintext
    string text = get_string("plaintext: ");

    // combine plaintext and key to generate ciphertext
    string ciphertext = substitution_cipher(text, key);

    // print ciphertext
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}


// return true if key contain nonalphabetic characters
// otherwise return false
bool check_non_alphabetic(string key, int length)
{
    for (int i = 0; i < length; i++)
    {
        // check if character is not alphabetical
        if (!isalpha(key[i]))
        {
            return true;
        }
    }

    return false;
}


// return true if key contains duplicate characters
// otherwise return false
bool check_duplicate(string key, int length)
{
    char temp_array[length];

    // iterate through each letter in key
    for (int i = 0; i < length; i++)
    {
        char test_char = key[i];

        // iterate through temp_array checking that test_char does not already exist in temp_array
        // if test_char matches a letter in temp_array, then it is a duplicate
        for (int j = 0; j < length; j++)
        {
            if (test_char == temp_array[j])
            {
                // duplicate found
                return true;
            }
        }

        // add to temp_array for next comparisons
        temp_array[i] = test_char;
    }

    return false;
}


// substitute alphabetical letters in text with the corresponding letter in key
// preserves initial lettercase state
string substitution_cipher(string text, string key)
{
    // copy plaintext into ciphertext as a starting point
    string ciphertext = text;

    // iterate through each character of text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // if char is a letter, then substitute
        if (isalpha(text[i]))
        {
            // determine key index
            // using lowercase 'a' = 97
            int key_index = tolower(text[i]) - 97;

            // get char from key
            char cipher_char = key[key_index];

            // covert to lowercase/uppercase
            if (isupper(text[i]))
            {
                cipher_char = toupper(cipher_char);
            }
            else
            {
                cipher_char = tolower(cipher_char);
            }

            // substitute
            ciphertext[i] = cipher_char;
        }
    }

    return ciphertext;
}