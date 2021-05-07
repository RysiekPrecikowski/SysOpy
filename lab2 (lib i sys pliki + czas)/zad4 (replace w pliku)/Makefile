CC = gcc
CFLAGS = -Wall -std=c11 -g

clean:
	rm -f *.o *.a *.so main out outTest.txt file*.txt

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

run: compile
	./main a out ty aaa

test: compile
	./generator 1 10000000 file
	./main file0.txt outTest.txt lib SysOpy

