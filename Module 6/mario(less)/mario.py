from cs50 import get_int

while True:
    high = get_int("Qual a altura da piramide")
    if high > 0 and high < 9:
        break

for v in range(high):
    for spaces in range(1, (high - v), 1):
        print(' ', end = '')
    for j in range(0, (v + 1), 1):
        print('#', end = '')
    print()