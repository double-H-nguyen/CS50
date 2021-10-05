from cs50 import get_int

scores = []
for i in range(3):
    user_input = get_int("Score: ")
    scores.append(user_input)

avg = sum(scores) / len(scores)
print(f"Average: {avg}")