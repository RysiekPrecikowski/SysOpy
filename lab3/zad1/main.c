#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Napisz program, w którym z procesu macierzystego stworzysz
 * dokładnie n procesów potomnych (n parametr wywołania programu).
 * W każdym z procesów potomnych wypisz komunikat, w którym poinformujesz
 * z którego procesu napis pochodzi.
 */

int main(int argc, char* argv[]){
    if (argc != 2)
        return -1;
    int n = atoi(argv[1]);
    pid_t parent_pid = getpid();
    printf("parent pid: %d\n\n", parent_pid);

    for (int i = 0 ; i < n ; i++){
        if (getpid() == parent_pid){
            fork();
        }
    }

    if (getpid() != parent_pid)
        printf("napis z procesu: %d, a jego rodzic to: %d\n", getpid(), getppid());

    if(getpid() == parent_pid)
        while (wait(NULL) > 0);

    return 0;
}