CC = gcc
CFLAGS = -Wall -std=c11 -g -D_BSD_SOURCE

run: compile
	./main . ":)" 2

clean:
	rm -f *.o *.a *.so main *.tmp

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main


