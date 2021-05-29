CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run_server: compile
	./server

run_client: compile
	./client

clean:
	rm -f *.o *.a *.so server client

compile:
	make clean
	$(CC) $(CFLAGS) server.c -o server -lpthread
	$(CC) $(CFLAGS) client.c -o client -lpthread
