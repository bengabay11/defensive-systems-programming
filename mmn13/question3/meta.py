from functools import wraps
from importlib.machinery import SourceFileLoader
import inspect
import os
import sys
from typing import Callable


class Replicator:
    def __init__(self, python_code: str) -> None:
        self.python_code = python_code

    def decorator(self, func):
        @wraps(func)
        def inner(*args, **kwargs):
            eval(self.python_code)
            return func(*args, **kwargs)
        return inner

    def affect(self, cls):
        for attr, item in cls.__dict__.items():
            if callable(item):
                setattr(cls, attr, self.decorator(item))


def get_first_class_from_file(python_filename: str) -> any:
    module = SourceFileLoader(os.path.basename(
        python_filename), python_filename).load_module()
    file_classes = [obj for name, obj in inspect.getmembers(
        module) if inspect.isclass(obj)]
    if file_classes:
        return file_classes[0]
    raise Exception(f"No classes found in file - '{python_filename}'")


def init_dynamic_params(function: Callable, str_value: str, int_value: int) -> dict:
    parameter_values = {}
    for parameter_name, parameter_type in function.__annotations__.items():
        if parameter_name != "return":
            if parameter_type == str:
                parameter_values[parameter_name] = str_value
            elif parameter_type == int:
                parameter_values[parameter_name] = int_value
    return parameter_values


def call_class_methods(cls_instance: any) -> None:
    for member_name, member in inspect.getmembers(cls_instance):
        if inspect.ismethod(member) and member_name != "__init__":
            parameter_values = init_dynamic_params(member, "str_param", 1)    
            result = member(**parameter_values)
            if result:
                print(result)


def create_dynamic_class_instance(cls: any, str_value: str, int_value: int) -> any:
    init_function = dict(inspect.getmembers(cls)).get("__init__")
    parameter_values = init_dynamic_params(init_function, str_value, int_value)
    return cls(**parameter_values)


def main() -> None:
    python_filename = input("Enter python file name: ")
    first_class = get_first_class_from_file(python_filename)
    if first_class is None:
        print(f"No class found in file {python_filename}.")
        sys.exit(1)
    first_class_instance1 = create_dynamic_class_instance(first_class, "red", 4)
    first_class_instance2 = create_dynamic_class_instance(first_class, "blue", 50)
    python_code = input("Enter a python code: ")
    replicator = Replicator(python_code)
    replicator.affect(first_class)

    call_class_methods(first_class_instance1)
    call_class_methods(first_class_instance2)


if __name__ == '__main__':
    main()
