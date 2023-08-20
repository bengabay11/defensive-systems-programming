

class BankAccount:
    def __init__(self, account_name: str, balance: int):
        self.name = account_name
        self.amt = balance

    def __str__(self):
        return f"Your account, {self.name}, has {self.amt} dollars."


def main():
    t1 = BankAccount("Bob", 100)
    print(t1)


if __name__ == '__main__':
    main()
