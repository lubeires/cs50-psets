import cs50


def main():
    credit = cs50.get_string("Number: ")
    valid_sum = check_sum(credit)
    length = len(credit)
    first_digits = int(credit[0:2])

    if valid_sum:
        if length == 15 and (first_digits == 34 or first_digits == 37):
            print("AMEX")
        elif length == 16 and first_digits >= 51 and first_digits <= 55:
            print("MASTERCARD")
        elif length == 16 or length == 13 and first_digits / 10 == 4:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")


def check_sum(credit):
    i = sum = 0
    for digit in credit[::-1]:
        if i % 2 == 0:
            sum += int(digit)
        else:
            digit = str(int(digit) * 2)
            for d in digit:
                sum += int(d)
        i += 1
    return True if sum % 10 == 0 else False


main()