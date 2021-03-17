CC = gcc
OPT = -O0
CFLAGS = -Wall -Werror -std=c11 -g $(OPT)


static:
	make clean
	$(CC) $(CFLAGS) -c myLib.c -o myLib.o
	ar rcs libmyLib.a myLib.o
	$(CC) $(CFLAGS) main.c -l myLib -L ./ -o main
	@echo "\n#######   static lib tests   ########\n" >> results3b.txt
	make --no-print-directory -f tests runTests

shared:
	make clean
	$(CC) $(CFLAGS) -fPIC -shared myLib.c -o libmyLib.so
	$(CC) $(CFLAGS) main.c -L. -Wl,-rpath=. -l myLib -o main
	@echo "\n#######   shared lib tests   ########\n" >> results3b.txt
	make --no-print-directory -f tests runTests

dynamic:
	make clean
	$(CC) $(CFLAGS) -fPIC -shared myLib.c -o libmyLib.so
	$(CC) $(CFLAGS) main.c -ldl -D DYNAMIC -o main
	@echo "\n#######   dynamic lib tests   ########\n" >> results3b.txt
	make --no-print-directory -f tests runTests

clean:
	rm -f *.o *.a *.so file*.txt merged.txt main


libtests:
	make --no-print-directory static
#	make --no-print-directory shared
#	make --no-print-directory dynamic

all:
	@rm -f results3b.txt

	@echo "--------------- optimisation O0 --------------------\n" >> results3b.txt
	make --no-print-directory libtests OPT=-O0

	@echo "--------------- optimisation O2 --------------------\n" >> results3b.txt
	make --no-print-directory libtests OPT=-O2

	@echo "--------------- optimisation Os --------------------\n" >> results3b.txt
	make --no-print-directory libtests OPT=-Os