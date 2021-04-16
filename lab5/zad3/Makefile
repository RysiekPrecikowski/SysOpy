CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main
clean:
	rm -f *.o *.a *.so main producer

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) producer.c -o producer
	$(CC) $(CFLAGS) consumer.c -o consumer

