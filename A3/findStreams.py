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

def getStreamList(name):
    streamList = []
    for fileStr in os.listdir("messages"):
        if fnmatch.fnmatch(fileStr, "*Users"):
            with open("messages/" + fileStr) as file:
                for line in file:
                    if line.split("\t")[0] == name:
                        streamList.append(fileStr)

    for i in range(0, len(streamList)):
        streamList[i] = streamList[i][:-11]

    return streamList

def main():
    if len(sys.argv) < 2:
        print("Error: Please enter a user name")
        exit()

    name = ""
    for args in sys.argv[1:]:
        name += args
        name += " "
    name = name[:len(name)-1]

    streamList = getStreamList(name)

    for stream in streamList:
        print(stream)

if __name__ == "__main__":
    main()
