CC = gcc
CFLAGS = -Wall -std=c11 -g

run: compile
	./main merge_files a:b merge_files aa:bb merge_files aa:a merge_files bb:b

clean:
	rm -f *.o *.a *.so main *.tmp

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main

