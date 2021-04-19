CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./client &
	./server


clean:
	rm -f *.o *.a *.so main server client

compile:
	make clean
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

