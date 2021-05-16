CC = gcc
CFLAGS = -Wall -g -Werror=implicit-function-declaration
n = 4
run: compile
	./main dog $(n) block
	./main dog $(n) numbers

clean:
	rm -f *.o *.a *.so main

compile:
	make clean
	$(CC) $(CFLAGS) main.c -o main -lpthread

test_block: compile
	for threads in 1 2 4 8 16; do \
		echo ; \
		echo "**************** $$threads threads *****************\n" ; \
		./main dog $$threads block ; \
	done


test_numbers: compile
	for threads in 1 2 4 8 16; do \
		echo ; \
		echo "**************** $$threads threads *****************\n" ; \
		./main dog $$threads numbers ; \
	done

all_tests:
	@echo "###################################"
	@echo "############  NUMBERS #############"
	@echo "###################################"

	@make -s test_numbers

	@echo
	@echo "###################################"
	@echo "#############  BLOCK ##############"
	@echo "###################################"

	@make -s test_block
