#!/usr/bin/python3
"/********************************"
"Author: Dimitar Dimitrov 0922289"
"Date: March 17, 2017"
"Course: CIS 2750 A3"
"*********************************/"

import sys
import os
import fnmatch
import fileinput

def main():
    if len(sys.argv) < 2:
        print("Error: Please enter a user name")
        exit()

    name = ""
    for args in sys.argv[1:]:
        name += args
        name += " "
    name = name[:len(name)-1]

    command = "./db -find "
    command = command + "\"" + name + "\""
    os.system(command);

if __name__ == "__main__":
    main()
