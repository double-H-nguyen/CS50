#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int count_letters(string);
int count_words(string);
int count_sentences(string);


int main(void)
{
    // prompt user for text
    string text = get_string("Text: ");

    // calculate number of letters, words, and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // L = average number of letters per 100 words in the text
    // S = average number of sentences per 100 words in the text
    double L = (double) letters * 100 / words;
    double S = (double) sentences * 100 / words;

    // use Coleman-Liau formula
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}


// assume that a letter is any lowercase character from a to z or any uppercase character from A to Z
int count_letters(string text)
{
    int letters_ct = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters_ct++;
        }
    }

    return letters_ct;
}


// consider any sequence of characters separated by a space to be a word
int count_words(string text)
{
    int words_ct = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i]))
        {
            words_ct++;
        }
    }

    // add one to account for the last word in the text
    words_ct++;

    return words_ct;
}


// consider any sequence of characters that ends with a . or a ! or a ? to be a sentence
int count_sentences(string text)
{
    int sentences_ct = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences_ct++;
        }
    }

    return sentences_ct;
}