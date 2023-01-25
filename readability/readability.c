#include <ctype.h>
#include <math.h>
#include <cs50.h>
#include <stdio.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float l = 100 * (float)letters / words;
    float s = 100 * (float)sentences / words;
    int grade = round(0.0588 * l - 0.296 * s - 15.8);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int i, count;
    i = count = 0;
    while (text[i] != '\0')
    {
        if (tolower(text[i]) >= 97 && tolower(text[i]) <= 122)
        {
            count++;
        }
        i++;
    }
    return count;
}

int count_words(string text)
{
    int i, count;
    i = count = 0;
    while (text[i] != '\0')
    {
        if (text[i] == 32)
        {
            count++;
        }
        i++;
    }
    return ++count;
}

int count_sentences(string text)
{
    int i, count;
    i = count = 0;
    while (text[i] != '\0')
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            count++;
        }
        i++;
    }
    return count;
}