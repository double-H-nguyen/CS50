# print vertical block
for i in range(3):
    print("#")
print()

# print ? block
for i in range(4):
    print("?", end="")
print()

# another way to print ? block
print()
print("?" * 4)

# print block
for i in range(3): # row
    for j in range(3): # column
        print("#", end="")
    print()