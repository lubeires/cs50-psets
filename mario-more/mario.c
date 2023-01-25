#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;

    // gets height
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    for (int i = 0; i < h; i++)
    {
        // whitespaces before each
        for (int j = 1; j < h - i; j++)
        {
            printf(" ");
        }
        // left side of the pyramid
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        // whitespace between sides
        printf("  ");
        // left side of the pyramid
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        // next level
        printf("\n");
    }
}