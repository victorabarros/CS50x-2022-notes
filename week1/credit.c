#include <cs50.h>
#include <stdio.h>

int calculate_checksum(long number);
int sum_digits(int number);
string check_brand(long number);
int number_length(long number);
int first_two_digits(long number);

int main(void)
{
    long number = get_long("Number: ");
    int checksum = calculate_checksum(number);

    if (checksum % 10 == 0)
    {
        printf("%s\n", check_brand(number));
    }
    else
    {
        printf("INVALID\n");
    }
}

int calculate_checksum(long number)
{
    int pair_digits_sum_double_digits = 0;
    int odd_digits_sum = 0;
    bool is_odd_digit = true;

    do
    {
        // get last digit
        int digit = number % 10;
        if (is_odd_digit)
        {
            odd_digits_sum += digit;
        }
        else
        {
            int double_digit = 2 * digit;
            pair_digits_sum_double_digits += sum_digits(double_digit);
        }

        // remove last digit to continue iteration
        number = (number - digit) / 10;
        is_odd_digit = !is_odd_digit;
    }
    while (number > 0);

    return odd_digits_sum + pair_digits_sum_double_digits;
}

int sum_digits(int number)
{
    int digits_sum = 0;

    do
    {
        int digit = number % 10;
        digits_sum += digit;

        number = (number - digit) / 10;
    }
    while (number > 0);

    return digits_sum;
}

string check_brand(long number)
{
    int length = number_length(number);
    int two_digits = first_two_digits(number);

    // apply lenght and digits rules to return what brand is
    if (length == 15 && (two_digits == 34 || two_digits == 37))
    {
        return "AMEX";
    }
    else if (length == 13 && (two_digits >= 40 && two_digits < 50))
    {
        return "VISA";
    }
    else if (length == 16)
    {

        if (two_digits >= 40 && two_digits < 50)
        {
            return "VISA";
        }
        else if (two_digits >= 51 && two_digits < 56)
        {
            return "MASTERCARD";
        }

    }

    return "INVALID";
}

int number_length(long number)
{
    int length = 0;

    do
    {
        int digit = number % 10;

        number = (number - digit) / 10;
        length++;
    }
    while (number > 0);

    return length;
}

int first_two_digits(long number)
{
    do
    {
        int digit = number % 10;
        number = (number - digit) / 10;
    }
    while (number > 100);

    return number;
}
