

def main():
    words = ["adopt", "bake", "beam", "cook", "time",
             "grill", "waved", "hire"]  # put your code below

    past_tense = []
    for word in words:
        if word.endswith("ed"):
            past_tense.append(word)
        elif word.endswith("e"):
            past_tense.append(word + "d")
        else:
            past_tense.append(word + "ed")

    print(past_tense)


if __name__ == "__main__":
    main()
