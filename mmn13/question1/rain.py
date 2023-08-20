

def main():
    Rainfull_mi = [45, 65, 70.4, 82.6, 20.1, 90.8, 76.1,
                   30.92, 46.8, 67.1, 79.9]  # put your code below

    num_rainy_months = 0
    for x in Rainfull_mi:
        if x > 75:
            num_rainy_months += 1

    print(f"There are {num_rainy_months} months of rainy weather")


if __name__ == "__main__":
    main()
