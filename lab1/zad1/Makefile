CC = gcc
CFLAGS = -Wall -Werror -std=c11 -g

all: static shared

static:
	make clean
	$(CC) $(CFLAGS) -c myLib.c -o myLib.o
	ar rcs myLib.a myLib.o

shared:
	make clean
	$(CC) $(CFLAGS) -fPIC -shared myLib.c -o myLib.so

clean:
	rm -f *.o *.a *.so file*.txt merged.txt main
