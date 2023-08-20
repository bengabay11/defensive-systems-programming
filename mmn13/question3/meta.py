from importlib.machinery import SourceFileLoader
import inspect
import os
import sys


class Replicator:
    def deco(self, func, code):
        def ret(*args, **kwargs):
            eval(code)
            return func(*args, **kwargs)
        return ret

    def affect(self, cls, code):
        for attr, item in cls.__dict__.items():
            if callable(item):
                setattr(cls, attr, self.deco(item, code))


def get_first_class_from_file(python_filename: str) -> any:
    module = SourceFileLoader(os.path.basename(
        python_filename), python_filename).load_module()
    return [obj for name, obj in inspect.getmembers(
        module) if inspect.isclass(obj)][0]


def call_class_methods(cls_instance):
    for name, item in inspect.getmembers(cls_instance):
        if inspect.ismethod(item) and name != "__init__":
            result = item()
            if result:
                print(result)


def main():
    python_filename = input("Enter python file name: ")
    first_class = get_first_class_from_file(python_filename)
    if first_class is None:
        print(f"No class found in file {python_filename}.")
        sys.exit(1)
    first_class_instance1 = first_class("red", 4)
    first_class_instance2 = first_class("blue", 50)
    python_code = input("Enter a python code: ")
    replicator = Replicator()
    replicator.affect(first_class, python_code)

    call_class_methods(first_class_instance1)
    call_class_methods(first_class_instance2)


if __name__ == '__main__':
    main()
