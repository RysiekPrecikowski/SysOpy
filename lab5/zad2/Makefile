CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main Rysiek test_programu jebac_sysopy
	./main sender
	./main sender
clean:
	rm -f *.o *.a *.so main

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

