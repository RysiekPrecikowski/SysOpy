CC = gcc
CFLAGS = -Wall -std=gnu11 -g

run_fork: compile
	@echo "\n**************************************"
	@echo "***********     FORK     *************"
	@echo "**************************************\n"
	@echo "****     ignore     ****"
	@./main_fork ignore
	@echo "****     handler     ****"
	@./main_fork handler
	@echo "****     mask     ****"
	@./main_fork mask
	@echo "****     pending     ****"
	@./main_fork pending


run_exec: compile
	@echo "\n**************************************"
	@echo "***********     EXEC     *************"
	@echo "**************************************\n"
	@echo "****     ignore     ****"
	@./main_exec ignore
	@#echo "****     handler     ****"
	@#./main_exec handler #handler not inherited in exec
	@echo "****     mask     ****"
	@./main_exec mask
	@echo "****     pending     ****"
	@./main_exec pending

clean:
	rm -f *.o *.a *.so main_fork main_exec child

compile: clean
	$(CC) $(CFLAGS) main_fork.c -o main_fork
	$(CC) $(CFLAGS) child.c -o child
	$(CC) $(CFLAGS) main_exec.c -o main_exec

all: run_fork run_exec