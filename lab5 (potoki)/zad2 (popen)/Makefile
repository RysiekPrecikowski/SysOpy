CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main Rysiek test_programu Testowa_wiadomosc
	./main sender
	./main date
clean:
	rm -f *.o *.a *.so main

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

