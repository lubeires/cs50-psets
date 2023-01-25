#include <cs50.h>
#include <stdio.h>
#include <math.h>

bool checkSum(long credit)
{
    int digit, i, sum;
    i = sum = 0;
    while (credit)
    {
        digit = credit % 10;
        if (i % 2 == 0)
        {
            sum += digit;
        }
        else
        {
            digit *= 2;
            while (digit)
            {
                sum += digit % 10;
                digit /= 10;
            }
        }
        credit /= 10;
        i++;
    }
    printf("SOMA: %i\n", sum);
    bool sumcheck = sum % 10 == 0 ? true : false;
    return sumcheck;
}

int digitsCounter(long credit)
{
    int digits = 0;
    while (credit)
    {
        credit /= 10;
        digits++;
    }
    return digits;
}

int firstDigits(long credit, int digits)
{
    return credit / pow(10, (digits - 2));
}

int main(void)
{
    long credit;
    // gets card number
    do
    {
        credit = get_long("Number: ");
    }
    while (!credit);
    // does checksum
    bool checksum = checkSum(credit);
    // counts the digits
    int digits = digitsCounter(credit);
    // gets the first two digits
    int first_digits = firstDigits(credit, digits);

    printf("%i\n", digits);
    printf("%i\n", first_digits);
    printf("%i\n", checksum);

    if (checksum)
    {
        // checks if it's AMEX
        if (digits == 15 && (first_digits == 34 || first_digits == 37))
        {
            printf("AMEX\n");
        }
        // checks if it's MASTERCARD
        else if (digits == 16 && (first_digits >= 51 && first_digits <= 55))
        {
            printf("MASTERCARD\n");
        }
        // checks if it's VISA
        else if ((digits == 16 || digits == 13) && (first_digits / 10 == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }



}