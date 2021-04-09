#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include "utils.c"

#define or ||

int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Wrong number of arguments!\n");
        return -1;
    }
    sigset_t set; // new set - blocked signals
    bool check_pending = false;

    printf("\nparent PID: %d\n\n", getpid());
    if (strcmp(argv[1], "ignore") == 0 or strcmp(argv[1], "handler") == 0){
        if (strcmp(argv[1], "ignore") == 0)
            signal(SIGUSR1, SIG_IGN);
        else
            signal(SIGUSR1, handler);
    } else if (strcmp(argv[1], "mask") == 0 or strcmp(argv[1], "pending") == 0) {
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);

        check_pending = true;
    } else {
        printf("Not recognized argument");
        return -1;
    }

    printf("Raising signal in main PID: %d\n", getpid());
    raise(SIGUSR1);
    check_what_happened();

    if (check_pending)
        check_if_pending(&set);

    if (fork() == 0) {
        if (strcmp(argv[1], "pending") == 0) {
            printf("Child process tests pending signals PID: %d\n", getpid());
        } else {
            printf("Raising signal in child PID: %d\n", getpid());
            raise(SIGUSR1);
            check_what_happened();
        }
        if (check_pending)
            check_if_pending(&set);
    }
    else {
        wait(NULL);
        printf("\n");
    }

    return 0;
}