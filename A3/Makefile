all: post addauthor parser
addauthor: addauthor.o libstream.a
	gcc addauthor.o -o addauthor -lstream -L.
addauthor.o: addauthor.c
	gcc -g -c -ansi -Wall -std=c99 addauthor.c
post: post.o libstream.a
	gcc post.o -o post -lstream -L.
libstream.a: stream.c stream.h
	gcc -c stream.c -o stream.o
	ar cr libstream.a stream.o
post.o: post.c
	gcc -g -c -ansi -Wall -std=c99 post.c
post.c: a1 post.cc
	./a1 post.cc
a1: a1.o
	gcc a1.o -o a1
a1.o: a1.c
	gcc -g -c -ansi -Wall -std=c99 a1.c
parser: parser.o
	gcc parser.o -o parser
parser.o: parser.c
	gcc -g -c -ansi -Wall -std=c99 parser.c -o parser.o
clean:
	rm *.o a1 post libstream.a tokens.txt post.c addauthor parser messages/*
