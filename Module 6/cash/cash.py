from cs50 import get_float

while True:
    cash = get_float('Change owed: ')
    if cash > 0:
        break

c25 = cash // 0.25
c10 = round(cash % 0.25, 2) // 0.10
c5 = round((cash % 0.25) % 0.10, 2) // 0.05
c1 = round(round(round(cash % 0.25, 2) % 0.10, 2) % 0.05, 2) // 0.01

print(int(c25 + c10 + c5 + c1))