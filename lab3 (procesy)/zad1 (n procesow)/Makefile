CC = gcc
CFLAGS = -Wall -std=c11 -g

run: compile
	./main 10

clean:
	rm -f *.o *.a *.so main

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

