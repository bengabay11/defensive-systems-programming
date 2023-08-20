

class AppleBasket:
    def __init__(self, color: str, quantity: int) -> None:
        self.apple_color = color
        self.apple_quantity = quantity

    def __str__(self) -> str:
        return f"A basket of {self.apple_quantity} {self.apple_color} apples."

    def increase(self) -> None:
        self.apple_quantity += 1


def main() -> None:
    apple_basket1 = AppleBasket("red", 4)
    apple_basket2 = AppleBasket("blue", 50)
    print(apple_basket1)
    print(apple_basket2)


if __name__ == "__main__":
    main()
