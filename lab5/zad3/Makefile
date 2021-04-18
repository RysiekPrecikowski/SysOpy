CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run: compile
	./main
clean:
	rm -f *.o *.a *.so main producer consumer

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) producer.c -o producer
	$(CC) $(CFLAGS) consumer.c -o consumer

test: compile
	./main mkfifo amount_of_consumers 3 amount_of_producers 4 0 1 2 3

test2: compile
	./main mkfifo amount_of_consumers 3 amount_of_producers 4 0 1 2 3
	./main mkfifo not_prepare amount_of_consumers 6 amount_of_producers 4 3 2 1 0
	./main mkfifo not_prepare amount_of_consumers 2 amount_of_producers 4 0 1 2 3

