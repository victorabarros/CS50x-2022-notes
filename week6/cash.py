from cs50 import get_float


def main():
    change = 0
    while (change <= 0):
        change = get_float("Change owed: ")

    cents = int(change * 100)
    quarters = calculate_quarters(cents)
    cents = cents - quarters * 25

    dimes = calculate_dimes(cents)
    cents = cents - dimes * 10

    nickels = calculate_nickels(cents)
    cents = cents - nickels * 5

    pennies = calculate_pennies(cents)

    print(quarters + dimes + nickels + pennies)


def calculate_quarters(cents):
    return cents // 25


def calculate_dimes(cents):
    return cents // 10


def calculate_nickels(cents):
    return cents // 5


def calculate_pennies(cents):
    return cents


main()
