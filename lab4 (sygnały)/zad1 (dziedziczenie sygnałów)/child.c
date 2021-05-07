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
    if (argc != 2){
        printf("Wrong number of arguments!\n");
        return -1;
    }

    if (strcmp(argv[1], "pending") == 0) {
        printf("Child process tests pending signals PID: %d\n", getpid());

    } else {
        printf("Raising signal in child PID: %d\n", getpid());
        raise(SIGUSR1);
        check_what_happened();
    }


    if (strcmp(argv[1], "pending") == 0 or strcmp(argv[1], "mask") == 0) {
        sigset_t set;
        check_if_pending(&set);
    }

    printf("\n");
    return 0;
}
