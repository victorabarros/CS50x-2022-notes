from cs50 import get_int


def main():
    number = get_int("Number: ")
    checksum = calculate_checksum(number)

    if (checksum % 10 == 0):
        print(check_brand(number))
    else:
        print("INVALID")


def calculate_checksum(number):
    pair_digits_sum_double_digits = 0
    odd_digits_sum = 0
    is_odd_digit = True

    while number > 0:
        # get last digit
        digit = number % 10
        if (is_odd_digit):
            odd_digits_sum += digit
        else:
            double_digit = 2 * digit
            pair_digits_sum_double_digits += sum_digits(double_digit)

        # remove last digit to continue iteration
        number = (number - digit) / 10
        is_odd_digit = not is_odd_digit

    return odd_digits_sum + pair_digits_sum_double_digits


def sum_digits(number):
    digits_sum = 0

    while number > 0:
        digit = number % 10
        digits_sum += digit
        number = (number - digit) / 10

    return digits_sum


def check_brand(number):
    length = number_length(number)
    two_digits = first_two_digits(number)

    # apply lenght and digits rules to return what brand is
    if (length == 15 and (two_digits == 34 or two_digits == 37)):
        return "AMEX"
    elif (length == 13 and (two_digits >= 40 and two_digits < 50)):
        return "VISA"
    elif (length == 16):
        if (two_digits >= 40 and two_digits < 50):
            return "VISA"
        elif (two_digits >= 51 and two_digits < 56):
            return "MASTERCARD"

    return "INVALID"


def number_length(number):
    length = 0

    while number > 0:
        digit = number % 10
        number = (number - digit) / 10
        length += 1

    return length


def first_two_digits(number):
    while (number > 100):
        digit = number % 10
        number = (number - digit) / 10

    return number


if __name__ == "__main__":
    main()
