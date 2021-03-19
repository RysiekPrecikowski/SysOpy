CC = gcc
CFLAGS = -Wall -std=c11 -g

clean:
	rm -f *.o *.a *.so main a.txt b.txt c.txt

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

run: compile
	./main

test: compile
	./numbersGenerator 100000
	./main

