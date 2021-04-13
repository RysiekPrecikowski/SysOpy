CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main 10 KILL

clean:
	rm -f *.o *.a *.so main sender catcher

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) sender.c -o sender
	$(CC) $(CFLAGS) catcher.c -o catcher


