CC = gcc
CFLAGS = -Wall -std=c11 -g

clean:
	rm -f *.o *.a *.so main file*.txt

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

run: compile
	./main a b


test: compile
	@./generator 2 10000000 file

	./main file0.txt file1.txt