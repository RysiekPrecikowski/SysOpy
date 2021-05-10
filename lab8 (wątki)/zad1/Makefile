CC = gcc
CFLAGS = -Wall -g

run: compile
	./main brain 1 block

clean:
	rm -f *.o *.a *.so main

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main -lpthread

