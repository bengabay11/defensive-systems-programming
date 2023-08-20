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
        User.__init__(name, "Electrical Engineer")


class ComputerEngineer(Engineer):
    def __init__(self, name: str) -> None:
        User.__init__(name, "Computer Engineer")


class MechanicalEngineer(Engineer):
    def __init__(self, name: str) -> None:
        User.__init__(self, name, "Mechanical Engineer")


def get_live_class(class_name: str) -> any:
    for name, obj in inspect.getmembers(sys.modules[__name__]):
        if class_name == name and inspect.isclass(obj):
            return obj
    raise Exception(f"Class '{class_name}' not found in current module.")


def main():
    class_name = input("Please enter the name of new class: ")
    base_class_name = input("Please enter name of base class (blank if none): ")
    method_name = input(f"Please enter name of new method for class {class_name}: ")
    attribute_name = input(f"Please enter name of new attribute for class {class_name}:")

    base_classes = ()
    if base_class_name:
        base_class = get_live_class(base_class_name)
        base_classes = (base_class,)

    new_class = type(class_name, base_classes, {
        method_name: lambda: None,
        attribute_name: None
    })
    print(
        f"Class Student created with base class: {new_class.__bases__[0].__name__}")
    print(f"Class __name__ is: {new_class.__name__}")
    print(f"Class __dict__ is: {new_class.__dict__}")


if __name__ == "__main__":
    main()
