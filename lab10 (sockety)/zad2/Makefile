CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run_server: compile
	./server

run_client: compile
	./client $(NAME) $(TYPE)

clean:
	rm -f *.o *.a *.so server client server_socket

compile:
	#make clean
	$(CC) $(CFLAGS) server.c -o server -lpthread
	$(CC) $(CFLAGS) client.c -o client -lpthread
