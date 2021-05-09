CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./server &
	./client


server:
	$(CC) $(CFLAGS) server.c -o server
	./server

client:
	$(CC) $(CFLAGS) client.c -o client -lncurses
	./client

clean:
	rm -f *.o *.a *.so main server client

compile:
	#make clean
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

