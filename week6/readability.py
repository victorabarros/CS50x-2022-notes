from cs50 import get_string


def main():
    text = get_string("Text: ")
    index = colemanLiauIndex(text)

    if (index >= 16.0):
        print("Grade 16+")
    elif (index < 1):
        print("Before Grade 1")
    else:
        print(f"Grade {index:.0f}")


def colemanLiauIndex(text):
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    L = letters * 100.0 / words
    S = sentences * 100.0 / words

    return 0.0588 * L - 0.296 * S - 15.8


def count_letters(text):
    letters = 0

    for ii in text:
        if ii.isalnum():
            letters += 1

    return letters


def count_words(text):
    spaces = 0
    for ii in text:
        if ii == " ":
            spaces += 1

    return spaces + 1


def count_sentences(text):
    sentences = 0
    for ii in text:
        if (ii == '.' or ii == '!' or ii == '?'):
            sentences += 1

    return sentences


if __name__ == "__main__":
    main()
