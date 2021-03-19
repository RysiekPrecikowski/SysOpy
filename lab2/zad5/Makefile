CC = gcc
CFLAGS = -Wall -std=c11 -g

clean:
	rm -f *.o *.a *.so main out file*.txt outTest.txt

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

run: compile
	./main a out

test: compile
	./generator 1 1000000 file
	./main file0.txt outTest.txt

