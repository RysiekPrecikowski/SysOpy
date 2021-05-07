CC = gcc
CFLAGS = -Wall -std=c11 -g

clean:
	rm -f *.o *.a *.so main file*.txt

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

run: compile
	./main e a

test: compile
	./generator 1 1000000 file
	./main e file0.txt

