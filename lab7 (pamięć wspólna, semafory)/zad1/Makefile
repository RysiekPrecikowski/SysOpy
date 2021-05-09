CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main

clean:
	rm -f *.o *.a *.so main delivery_man cook

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) delivery_man.c -o delivery_man
	$(CC) $(CFLAGS) cook.c -o cook

