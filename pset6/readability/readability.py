import cs50


def main():
    # Prompt user for text
    text = cs50.get_string("Text: ")

    # Calculate number of letters, words, and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # L = average number of letters per 100 words in the text
    # S = average number of sentences per 100 words in the text
    L = letters * 100 / words
    S = sentences * 100 / words

    # Use Coleman-Liau formula
    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Assume that a letter is any lowercase character from a to z or any uppercase character from A to Z
def count_letters(text):
    letters_ct = 0

    for i in range(len(text)):
        if text[i].isalpha():
            letters_ct += 1

    return letters_ct


# Consider any sequence of characters separated by a space to be a word
def count_words(text):
    words_ct = 0

    for i in range(len(text)):
        if text[i].isspace():
            words_ct += 1

    # Add one to account for the last word in the text
    words_ct += 1

    return words_ct


# Consider any sequence of characters that ends with a . or a ! or a ? to be a sentence
def count_sentences(text):
    sentences_ct = 0

    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            sentences_ct += 1

    return sentences_ct


if __name__ == "__main__":
    main()
