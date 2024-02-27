from cs50 import get_int, get_string
import re

card_number = get_string('Number: ')
if len(card_number) in [13, 14, 15, 16]:
    if card_number[:2] in ["34", "37"]:
        print("AMEX")
    elif card_number[:2] in ["51", "52", "53", "54", "55"]:
        print("MASTERCARD")
    elif card_number[:1] == "4":
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")