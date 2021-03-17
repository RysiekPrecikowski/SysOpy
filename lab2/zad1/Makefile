CC = gcc
CFLAGS = -Wall -std=c11 -g

clean:
	rm -f *.o *.a *.so main
compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

run: compile
	./main a b
