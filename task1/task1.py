#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys

def main():
    try:
        n = sys.argv[1]
        m = sys.argv[2]
        lst = [n for n in range(int(n))][1:]
        print(lst)
    except IndexError:
        print("Error input")

if __name__ == "__main__":
    main()
