from cs50 import get_string
import re

text = get_string("Text: ").lower()
phrase = 0
letters = 0
words = 0

for j in range(len(text)):
    if text[j] in ['.', '?', '!']:
        phrase += 1
        if j == (len(text) - 1):
            words += 1
    if text[j] <= 'z' and text[j] >= 'a':
        letters += 1
    if text[j] == ' ':
        words += 1

L = (100 * letters) / words
S = phrase / (words/100)
readability = round((0.0588 * L) - (0.296 * S) - 15.8)

if readability <= 1:
    print("Before Grade 1")
elif readability >= 16:
    print("Grade 16+")
else:
    print(f"Grade {readability}")
print(f"{words} {phrase} {letters}")