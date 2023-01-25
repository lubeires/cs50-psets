# TODO
import cs50

h = cs50.get_int("Height: ")
while h not in range(1, 9):
    h = cs50.get_int("Height: ")

for i in range(h):
    for j in range(1, h - i):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print("  ", end='')
    for l in range(i + 1):
        print("#", end="")
    print()