import sys

names = ["Bill", "Charlie", "Fred", "George", "Ginny", "Percy", "Ron"]

if "Ron" in names:
    print("found")
    sys.exit(0)
else:
    print("Not found")
    sys.exit(1)
