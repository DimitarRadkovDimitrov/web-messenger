all: a1
a1: a1.o
	gcc a1.o -o a1
a1.o: a1.c
	gcc -g -c -ansi -Wall -std=c99 a1.c -Iinclude
clean:
	rm *.o
	rm a1
	rm tokens.txt
