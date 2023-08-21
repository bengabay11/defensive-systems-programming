Rainfull_mi = "45, 65, 70.4, 82.6, 20.1, 90.8, 76.1, 30.92, 46.8, 67.1, 79.9"

rainfall_amounts = Rainfull_mi.split(", ")

num_rainy_months = 0
for amount in rainfall_amounts:
    if float(amount) > 75:
        num_rainy_months += 1

if __name__ == "__main__":
    print(f"There are {num_rainy_months} months of rainy weather")
