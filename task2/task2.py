#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys


def main():
    try:
        if len(sys.argv) > 3:
            print("Error input")
            exit(1)

        file1 = sys.argv[1]
        with open(file1, 'r') as f1:
            text = f1.read()

        text_nums = text.split()
        c_x, c_y, r = float(text_nums[0]), float(text_nums[1]), float(text_nums[2])

        file2 = sys.argv[2]
        with open(file2, 'r') as f2:
            for line in f2:
                coord = line.split()
                x, y = float(coord[0]), float(coord[1])
                h = (x - c_x) ** 2 + (y - c_y) ** 2
                if str(h) == str(r ** 2):
                    print(0)
                elif h < r ** 2:
                    print(1)
                else:
                    print(2)

    except IndexError:
        print("Error input")


if __name__ == "__main__":
    main()
