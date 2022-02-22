#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys


def main():
    try:
        if len(sys.argv) > 3:
            print("Error input")
            exit(1)
        n = sys.argv[1]
        m = sys.argv[2]
        lst = int(m) * [int(i) for i in range(1, int(n) + 1)]
        lst2 = [' ']
        lst3 = []
        count = 0
        while lst2[-1] != 1:
            lst2.clear()
            for j in range(count, int(m) + count):
                lst2.append(lst[j])
                count += 1
            lst2c = lst2.copy()
            lst3.append(lst2c)
            count -= 1
        for k in range(len(lst3)):
            print(lst3[k][0], end='')

    except IndexError:
        print("Error input")


if __name__ == "__main__":
    main()
