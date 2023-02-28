with open('the-zen-of-python.txt') as f:
    for line in f:
        linesList = line.strip()

f.close()

f = open("time.txt", "w+") 
i=3
for x in linesList:
    if i % 3:
        f.write(x)
f.close()

f = open("speed.txt", "w+") 
i=4
for x in linesList:
    if i % 3:
        f.write(x)
f.close()

f = open("direction.txt", "w+") 
i=5
for x in linesList:
    if i % 3:
        f.write(x)
f.close()