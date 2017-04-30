#!/usr/bin/python3
"/********************************"
"Author: Dimitar Dimitrov 0922289"
"Date: March 17, 2017"
"*********************************/"

import sys
import os
import fnmatch
import fileinput

"getStreamList: returns a list of streams the user belongs to"
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

"tokenize: splits posts line by line"
def tokenize(posts):
    for i in range(0, len(posts)):
        posts[i] = posts[i].split("\n")
        posts[i] = posts[i][:len(posts[i])-1]
    return posts

"getPosts: opens stream and reads data file to retreive posts in their order"
def getPosts(streamname):
    file = open(streamname, "r")
    posts = []
    text = ""

    dataList = [0]
    for line in file:
        line = line[:len(line)-1]
        line = int(line)
        dataList.append(line)
    file.close()

    file = open(streamname[:-4], "r")

    for i in range(0, len(dataList)-1):
        file.seek(dataList[i])
        if (i+1) < len(dataList):
            text = file.read(dataList[i+1] - dataList[i])
        else:
            text = file.read()
        posts.append(text)

    file.close()
    return posts

"printPost: prints out a post message line by line"
def printPost(posts, stream, index, byName):
    if (posts):
        index = int(index)

        print("Stream: " + stream)

        for line in posts[index]:
            print(line)
    else:
        print("No posts to display")

"prevPost: iterates and changes index of post list to prev post"
def prevPost(posts, stream, index, byName):
    index = int(index)
    if index - 1 >= 0:
        index = index - 1

    print(index)
    printPost(posts, stream, index, byName)

"nextPost: iterates and changes index of post list to next post"
def nextPost(posts, username, stream, index, byName):
    index = int(index)

    if index + 1 < len(posts):
        index = index + 1

    if (stream != "all"):
        file = open("./messages/" + stream + "StreamUsers", "r")
        with fileinput.FileInput("./messages/" + stream + "StreamUsers", inplace=True) as file:
            for line in file:
                if line.split("\t")[0] == username:
                    num = line.split("\t")[1]

                    if index + 1 > int(num):
                        print(line.replace(num, str(index + 1)))
                    else:
                        print(line.replace(num, str(num[:-1])))
                else:
                    print(line, end = "")
        file.close()

    print(index)
    printPost(posts, stream, index, byName)


"markAll: sets number of posts read by user in user file to equal all posts"
def markAll(posts, username, stream, index, byName):
    if (stream != "all"):
        file = open("./messages/" + stream + "StreamUsers", "r")
        index = len(posts) - 1

        with fileinput.FileInput("./messages/" + stream + "StreamUsers", inplace=True) as file:
            for line in file:
                if line.split("\t")[0] == username:
                    num = line.split("\t")[1]
                    print(line.replace(num, str(len(posts))))
                else:
                    print(line, end = "")
        file.close()
    else:
        if not index:
            index = 0

    print(index)
    printPost(posts, stream, index, byName)

"getIndex: retrieves the very first index from"
def getIndex(username, stream, posts):
    if stream != "all":
        file = open("./messages/" + stream + "StreamUsers", "r")

        for line in file:
            if line.split("\t")[0] == username:
                index = int(line.split("\t")[1])
                if index == len(posts):
                    index = len(posts) - 1
                print(index)
                printPost(posts, stream, index, 0)

        with fileinput.FileInput("./messages/" + stream + "StreamUsers", inplace=True) as file:
            for line in file:
                if line.split("\t")[0] == username:
                    num = line.split("\t")[1]

                    if index + 1 > int(num):
                        print(line.replace(num, str(index+1)))
                    else:
                        print(line.replace(num, str(num[:-1])))
                else:
                    print(line, end = "")
        file.close()
    else:
        index = 0
        print(index)
        printPost(posts, stream, index, 0)

"main: ensures valid user input and calls helper functions"
def main():
    if len(sys.argv) < 2:
        print("Error: Please enter a user name")
        exit()

    if sys.argv[2] == "all":
        streams = getStreamList(sys.argv[1])
        posts = []
        for stream in streams:
            posts += getPosts("./messages/" + stream + "StreamData")
        posts = tokenize(posts)

    else:
        posts = getPosts("./messages/" + sys.argv[2] + "StreamData")
        posts = tokenize(posts)

    if len(sys.argv) > 4:
        if (sys.argv[4] == '-p'):
            prevPost(posts, sys.argv[2], sys.argv[3], sys.argv[5])
        elif (sys.argv[4] == '-n'):
            nextPost(posts, sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[5])
        elif (sys.argv[4] == '-m'):
            markAll(posts, sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[5])
    else:
        getIndex(sys.argv[1], sys.argv[2], posts)

if __name__ == "__main__":
    main()
