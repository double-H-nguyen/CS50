from cs50 import get_string

s = get_string("Before: ")
print("After: ", end="")

# print(s.upper())
for c in s:
    print(c.upper(), end="")
print()