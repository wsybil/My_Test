#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys
import math


def main():
    x = float()
    y = float()
    r = float()

    try:
        if len(sys.argv) > 3:
            print("Error input")
            exit(1)

        file1 = sys.argv[1]
        with open(file1, 'r') as f1:
            for line in f1:
                print(line)

        file2 = sys.argv[2]
        with open(file2, 'r') as f2:
            for line in f2:
                print(line)

    except IndexError:
        print("Error input")

    h = math.sqrt(x ** 2 + y ** 2)
    if h == r:
        print(0) # лежит
    elif h < r:
        print(1) # внутри
    else:
        print(2) # снаружи


if __name__ == "__main__":
    main()
