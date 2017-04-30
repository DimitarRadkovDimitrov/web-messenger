#!/usr/bin/python3
"/********************************"
"Author: Dimitar Dimitrov 0922289"
"Date: March 17, 2017"
"*********************************/"

import sys
import os
import fnmatch
import fileinput

"prevPost: iterates and changes index of post list to prev post"
def prevPost(username, stream, index, byName):
    command = "./db -view " + "\"" + username + "\" " + "\"" + stream + "\" " + index + " \"-p\""
    os.system(command)

"nextPost: iterates and changes index of post list to next post"
def nextPost(username, stream, index, byName):
    command = "./db -view " + "\"" + username + "\" " + "\"" + stream + "\" " + index + " \"-n\""
    os.system(command)

"markAll: sets number of posts read by user in user file to equal all posts"
def markAll(username, stream, index, byName):
    command = "./db -view " + "\"" + username + "\" " + "\"" + stream + "\" " + index + " \"-m\""
    os.system(command)

"getIndex: retrieves the very first index from"
def getIndex(username, stream):
    command = "./db -view " + "\"" + username + "\" " + "\"" + stream + "\""
    os.system(command)

"main: ensures valid user input and calls helper functions"
def main():
    if len(sys.argv) > 4:
        if (sys.argv[4] == '-p'):
            prevPost(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[5])
        elif (sys.argv[4] == '-n'):
            nextPost(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[5])
        elif (sys.argv[4] == '-m'):
            markAll(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[5])
    else:
        getIndex(sys.argv[1], sys.argv[2])

if __name__ == "__main__":
    main()
