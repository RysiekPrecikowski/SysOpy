#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig){
    printf("Received signal %d PID: %d, PPID: %d\n", sig, getpid(), getppid());
}

void check_what_happened(void){
    struct sigaction sa;
    sigaction(SIGUSR1, 0, &sa);
    printf("PID: %d ", getpid());
    if(sa.sa_handler==SIG_IGN)
        printf("ignored\n");
    else if(sa.sa_handler == handler)
        printf("user defined handler\n");
    else if(sa.sa_handler == SIG_BLOCK)
        printf("blocked\n");
    else
        printf("default ¯\\_(ツ)_/¯\n");
}

void check_if_pending(sigset_t *set){
    sigpending(set);
    if (sigismember(set, SIGUSR1))
        printf("Signal pending in PID: %d\n", getpid());
    else
        printf("Signal NOT pending in PID: %d\n", getpid());
}
