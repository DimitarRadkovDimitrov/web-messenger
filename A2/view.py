#!/usr/local/bin/python3
"/********************************"
"Author: Dimitar Dimitrov 0922289"
"Date: February 20, 2017"
"*********************************/"

import sys
import os
import fnmatch
import fileinput

"tokenize: splits posts line by line and ensures 80 character count"
def tokenize(posts):
    for i in range(0, len(posts)):
        posts[i] = posts[i].split("\n")
        posts[i] = posts[i][:len(posts[i])-1]

        for line in posts[i]:
            if len(line) >= 80:
                index = posts[i].index(line)
                string = line[:80]
                posts[i][index] = ""

                while len(string) >= 80:
                    posts[i][index] = posts[i][index] + string + "\n"
                    line = line[80:]
                    string = line[:80]
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

"getStreamList: returns a list of streams that a user belongs to"
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
        print(streamList[i] + " ", end = "")

    if not streamList:
        print("Error: User does not belong to stream")
        exit()
    print("all")

    return streamList

"view: navigates through posts using up and down arrow keys"
def view(username, posts, stream):
    file = open(stream, "r")
    buffer = []
    index = 0
    prev = 0
    blank = 0
    byName = False
    listByDate = list(posts)

    os.system("clear")
    for line in file:
        if line.split("\t")[0] == username:
            index = int(line.split("\t")[1])
            if index == len(posts):
                index = len(posts) - 1

    if index < len(posts) and len(posts) > 0:
        if len(posts[index]) < 22:
            blank = 22 - len(posts[index])
        else:
            buffer = posts[index][22:]

        print("Stream: " + stream[9:-11])
        for line in posts[index][:22]:
            print(line)

        if blank > 0:
            for i in range(0, blank):
                print(" ")

    print ("Page Up   Page Down   O-order toggle   M-mark all   S-stream  C-check for new")

    key = ""
    while key != "q":
        key = input()
        os.system("clear")

        if key == "\x1b[A":
            buffer[:] = []
            blank = 0

            if (index < len(posts)) and (index > 0):
                index = index - 1

            if (index < len(posts) - 1) and (index >= 0):
                if len(posts[index]) < 22:
                    blank = 22 - len(posts[index])
                else:
                    buffer = posts[index][22:]

                print("Stream: " + stream[9:-11])
                for line in posts[index][:22]:
                    print(line)

                if blank > 0:
                    for i in range(0, blank):
                        print(" ")

        if key == "\x1b[B":
            blank = 0
            if not buffer:
                if (index + 1) < len(posts):
                    index += 1

                with fileinput.FileInput(stream, inplace=True) as file:
                    for line in file:
                        if line.split("\t")[0] == username and byName == False:
                            num = line.split("\t")[1]

                            if index == len(posts)-1:
                                print(line.replace(num, str(index+1)))
                                prev = index + 1
                            else:
                                if prev <= index:
                                    print(line.replace(num, str(index)))
                                    prev = index
                                else:
                                    print(line.replace(num, num[:-1]))
                        else:
                            print(line, end = "")

                if (index < len(posts)) and (index >= 0):
                    if len(posts[index]) < 22:
                        blank = 22 - len(posts[index])

                        print("Stream: " + stream[9:-11])
                        for line in posts[index][:22]:
                            print(line)

                        if blank > 0:
                            for i in range(0, blank):
                                print(" ")
                    else:
                        buffer = posts[index][22:]

            else:
                if len(buffer) < 22:
                    blank = 22 - len(buffer)

                    for line in buffer:
                        print(line)

                    if blank > 0:
                        for i in range(0, blank):
                            print(" ")

                    buffer[:] = []
                else:
                    before = buffer[:22]

                    if len(before) < 22:
                        blank = 22 - len(before)

                    for line in before:
                        print(line)

                    if blank > 0:
                        for i in range(0, blank):
                            print(" ")
                    buffer = buffer[22:]

        if key == "m":
            with fileinput.FileInput(stream, inplace=True) as file:
                for line in file:
                    if line.split("\t")[0] == username:
                        num = line.split("\t")[1]
                        print(line.replace(num, str(len(posts))))
                    else:
                        print(line, end = "")

        if key == "s":
            streamList = getStreamList(username)
            newStream = input()
            posts = getPosts("messages/" + newStream + "StreamData")
            posts = tokenize(posts)
            view(username, posts, "messages/" + newStream + "StreamUsers")
            key = "q"
            os.system("clear")

        if key == "c":
            file = open(stream)
            for line in file:
                if line.split("\t")[0] == username:
                    if (int(line.split("\t")[1])) < len(posts):
                        index = index + 1

                        if index < len(posts):
                            if len(posts[index]) < 22:
                                blank = 22 - len(posts[index])
                            else:
                                buffer = posts[index][22:]

                            print("Stream: " + stream[9:-11])
                            for line in posts[index][:22]:
                                print(line)

                            if blank > 0:
                                for i in range(0, blank):
                                    print(" ")
                    else:
                        print("There are no new posts")

        if key == "o":
            if byName == False:
                posts.sort()
                byName = True

            elif byName == True:
                posts = list(listByDate)
                byName = False

            if index < len(posts):
                if len(posts[index]) < 22:
                    blank = 22 - len(posts[index])
                else:
                    buffer = posts[index][22:]

                print("Stream: " + stream[9:-11])
                for line in posts[index][:22]:
                    print(line)

                if blank > 0:
                    for i in range(0, blank):
                        print(" ")

        print ("Page Up   Page Down   O-order toggle   M-mark all   S-stream  C-check for new")

    file.close()

"main: ensures valid user input and calls helper functions"
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
    stream = input()

    isStream = 0
    for streams in streamList:
        if stream == streams:
            isStream = 1

    if isStream == 1:
        os.system("clear")
        posts = getPosts("messages/" + stream + "StreamData")
        posts = tokenize(posts)
        view(name, posts, "messages/" + stream + "StreamUsers")
    else:
        print("Error: Stream does not exist")
        exit()

if __name__ == "__main__":
    main()
