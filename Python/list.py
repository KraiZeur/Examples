l = [1,2,3,4,5,6,7,8,9,10]

len(l)

l.append(11)
cpy = l.copy()
count = l.count(11)


l.clear()

sliced = l[1:4]


for i in l:
    print(i)

if 11 in l:
    print("Found 11")
