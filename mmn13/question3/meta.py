import os


def main():
    python_filename = input("Enter python file name: ")
    python_added_code = input("Enter a python code: ")
    if os.path.isfile(python_filename):
        with open(python_filename, 'r') as python_file:
            



if __name__ == '__main__':
    main()
