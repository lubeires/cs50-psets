#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        // gets key
        string key = argv[1];
        // checks the key length
        if (strlen(key) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        for (int i = 0; i < strlen(key); i++)
        {
            // checks for any non-alphabetic character
            if (!isalpha(key[i]))
            {
                printf("Key must contain only alphabetic characters.\n");
                return 1;
            }
            // checks for any duplicated character
            for (int j = i + 1; j < strlen(key); j++)
            {
                if (tolower(key[i]) == tolower(key[j]))
                {
                    printf("Key must contain each letter exactly once.\n");
                    return 1;
                }
            }
        }

        string text;
        do
        {
            text = get_string("plaintext: ");
        }
        while (!text);

        int i = 0;
        while (text[i] != '\0')
        {
            if (isupper(text[i]))
            {
                text[i] = toupper(key[text[i] - 'A']);
            }
            else if (islower(text[i]))
            {
                text[i] = tolower(key[text[i] - 'a']);
            }
            i++;
        }

        printf("ciphertext: %s\n", text);
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    return 0;
}