from sys import argv
import csv

def main():
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    with open(argv[2]) as file:
        readed = file.read()
        #AGATC count
        AGATC = count(readed, 'AGATC')
        #TTTTTTCT count
        TTTTTTCT = count(readed, 'TTTTTTCT')
        #AATG count
        AATG = count(readed, 'AATG')
        #TCTAG count
        TCTAG = count(readed,'TCTAG')
        #GATA count
        GATA = count(readed, 'GATA')
        #TATC count
        TATC = count(readed, 'TATC')
        #GAAA count
        GAAA = count(readed, 'GAAA')
        #TCTG count
        TCTG = count(readed, 'TCTG')

    dnaanalyzed = [str(AGATC), str(TTTTTTCT), str(AATG), str(TCTAG), str(GATA), str(TATC), str(GAAA), str(TCTG)]
    dnaanalized_s = [str(AGATC), str(AATG), str(TATC)]
    with open(argv[1], "r") as persons:
        lines = csv.reader(persons)
        next(lines)
        for line in lines:
            j = 0
            if len(line)-1 == 3:
                for i in range(3):
                    if len(line)-1 == i:
                            break
                    if line[i+1] == dnaanalized_s[i]:
                        j += 1
                    if j == 3:
                        print(line[0])
                        exit(0)
            else:
                for i in range(8):
                    if len(line)-1 == i:
                            break
                    if line[i+1] == dnaanalyzed[i]:
                        j += 1
                    if j == 8:
                        print(line[0])
                        exit(0)
        print("No match")

def count(readed, str):
    count = []
    tmp = 1
    jumps = len(str)
    for i in range(len(readed)):
        if readed[i:i+jumps] == str:
            if readed[i-jumps:i] == str:
                tmp += 1
            else:
                count.append(tmp)
                tmp = 1
            i += jumps
    count.append(tmp)
    count.sort(reverse = True)
    return count[0]
main()