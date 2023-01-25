import cs50


text = cs50.get_string("Text: ")

letters = 0
for char in text:
    if char.isalpha():
        letters += 1
words = len(text.split(" "))
sentences = text.count(".") + text.count("!") + text.count("?")

l = 100 * letters / words
s = 100 * sentences / words
grade = round(0.0588 * l - 0.296 * s - 15.8)

if grade < 1:
    print("Before Grade 1")
elif grade >= 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")