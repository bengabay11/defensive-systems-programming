import sys
import inspect


class User:
    def __init__(self, name: str, profession: str) -> None:
        self.name = name
        self.profession = profession


class Engineer(User):
    def __init__(self, name: str) -> None:
        super().__init__(name, "Engineer")


class Technician(User):
    def __init__(self, name: str) -> None:
        super().__init__(name, "Technician")


class Author(User):
    def __init__(self, name: str) -> None:
        super().__init__(name, "Author")


class Politician(User):
    def __init__(self, name: str) -> None:
        super().__init__(name, "Politician")


class ElectricalEngineer(Engineer):
    def __init__(self, name: str) -> None:
        super().__init__(name)


class ComputerEngineer(Engineer):
    def __init__(self, name: str) -> None:
        super().__init__(name)


class MechanicalEngineer(Engineer):
    pass


def get_class(class_name: str) -> any:
    for name, obj in inspect.getmembers(sys.modules[__name__]):
        if class_name == name and inspect.isclass(obj):
            return obj


def main():
    class_name = input("Please enter the name of new class: ")
    base_class_name = input(
        "Please enter name of base class (blank if none): ")
    new_method_class = input(
        f"Please enter name of new method for class {class_name}: ")
    new_attribute_class = input(
        f"Please enter name of new attribute for class {class_name}:")
    base_class = get_class(base_class_name)
    if base_class:
        bases = (base_class,)
    else:
        bases = ()
    new_class = type(class_name, bases, {
                     new_method_class: None, new_attribute_class: None})
    # new_class_instance = new_class()
    print(
        f"Class Student created with base class: {new_class.__bases__[0].__name__}")
    print(f"Class __name__ is: {new_class.__name__}")
    print(f"Class __dict__ is: {new_class.__dict__}")


if __name__ == "__main__":
    main()
