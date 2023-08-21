

class AppleBasket:
    def __init__(self, apple_color: str, apple_quantity: int) -> None:
        self.apple_color = apple_color
        self.apple_quantity = apple_quantity

    def __str__(self) -> str:
        return f"A basket of {self.apple_quantity} {self.apple_color} apples."

    def increase(self) -> None:
        self.apple_quantity += 1

if __name__ == "__main__":
    apple_basket1 = AppleBasket("red", 4)
    apple_basket2 = AppleBasket("blue", 50)
    print(apple_basket1)
    print(apple_basket2)
