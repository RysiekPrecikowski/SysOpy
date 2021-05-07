CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main to_interprete

clean:
	rm -f *.o *.a *.so main

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

