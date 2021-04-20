CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./server &
	./client


server:
	./server

client: compile
	./client

clean:
	rm -f *.o *.a *.so main server client

compile:
	#make clean
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

