#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


bool receiving = true;
int counter = 0;


void handle_SIGUSR1(int sig){
    printf("sender got %d\n", sig);

    counter+=1;
}

void handle_SIGUSR2(int sig, siginfo_t *info, void *context){
    receiving = false;
}


int main(int argc, char* argv[]){ // "./sender", num_sigs, catcher_pid, mode
    printf("Sender PID: %d\n", getpid());
    pid_t catcher_pid = (pid_t) atoi(argv[2]);
    printf("Catcher PID in sender %d\n", catcher_pid);
    printf("Sender MODE: %s\n", argv[3]);

    char* mode = argv[3];
    int my_sig1 = SIGUSR1;
    int my_sig2 = SIGUSR2;

    if (strcmp(mode, "SIGRT") == 0){
        my_sig1 = SIGRTMIN + 1;
        my_sig2 = SIGRTMIN + 2;
    }


    signal(my_sig1, handle_SIGUSR1);
    struct sigaction act = {.sa_flags = SA_SIGINFO, .sa_sigaction = handle_SIGUSR2};
    sigaction(my_sig2, &act, NULL);

    int n = atoi(argv[1]);


    printf("Signals to send: %d\n", n);

    for (int i = 0 ; i < n ; i ++) {


        if (strcmp(mode, "SIGQUEUE") != 0) {
            union sigval val;
            sigqueue(catcher_pid, my_sig1, val);
        } else {
            kill(catcher_pid, my_sig1);
        }
    }
    if (strcmp(mode, "SIGQUEUE") != 0) {
        union sigval val;
        sigqueue(catcher_pid, my_sig2, val);
    } else {
        kill(catcher_pid, my_sig2);
    }

    printf("sender waiting\n");

    while (receiving);

    printf("sender received %d/%d signals\n", counter, n);

    return 0;
}