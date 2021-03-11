CC = gcc
CFLAGS = -Wall -Werror -std=c11 -g


static:
	make clean
	$(CC) $(CFLAGS) -c myLib.c -o myLib.o
	ar rcs libmyLib.a myLib.o
	$(CC) $(CFLAGS) main.c -l myLib -L ./ -o main
	@echo "\n#######   static lib tests   ########\n" >> results3a.txt
	make -s -f tests runTests

shared:
	make clean
	$(CC) $(CFLAGS) -fPIC -shared myLib.c -o libmyLib.so
	$(CC) $(CFLAGS) main.c -L. -Wl,-rpath=. -l myLib -o main
	@echo "\n#######   shared lib tests   ########\n" >> results3a.txt
	make -s -f tests runTests

dynamic:
	make clean
	$(CC) $(CFLAGS) -fPIC -shared myLib.c -o libmyLib.so
	$(CC) $(CFLAGS) main.c -ldl -D DYNAMIC -o main
	@echo "\n#######   dynamic lib tests   ########\n" >> results3a.txt
	make -s -f tests runTests

clean:
	rm -f *.o *.a *.so file*.txt merged.txt main


all:
	@rm -f results3a.txt
	make -s static
	make -s shared
	make -s dynamic