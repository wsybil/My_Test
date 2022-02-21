#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys

def main():
    try:
        n = sys.argv[1]
        m = sys.argv[2]
        
    except IndexError:
        print("Error input")

if __name__ == "__main__":
    main()
