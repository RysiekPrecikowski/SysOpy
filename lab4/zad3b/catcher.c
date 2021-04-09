#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

bool receiving = true;
int counter = 0;
pid_t sender_pid;
char* mode;

int my_sig1;
int my_sig2;

void handle_SIGUSR1(int sig, siginfo_t *info, void *context){
    printf("catcher got %d\n", sig);
    counter+=1;

    sender_pid = info->si_pid;

    printf("sending to %d\n", sender_pid);
    if (strcmp(mode, "SIGQUEUE") != 0) {
        union sigval val;
        sigqueue(sender_pid, my_sig1, val);
    } else {
        kill(sender_pid, my_sig1);
    }

}

void handle_SIGUSR2(int sig, siginfo_t *info, void *context){
    receiving = false;
    sender_pid = info->si_pid;
}


int main(int argc, char* argv[]){ // "./catcher", mode
    printf("Catcher PID: %d\n", getpid());

    mode = argv[1];

    my_sig1 = SIGUSR1;
    my_sig2 = SIGUSR2;

    if (strcmp(mode, "SIGRT") == 0){
        my_sig1 = SIGRTMIN + 1;
        my_sig2 = SIGRTMIN + 2;
    }

//    signal(my_sig1, handle_SIGUSR1);
    struct sigaction act1 = {.sa_flags = SA_SIGINFO, .sa_sigaction = handle_SIGUSR1};
    sigaction(my_sig1, &act1, NULL);

    struct sigaction act = {.sa_flags = SA_SIGINFO, .sa_sigaction = handle_SIGUSR2};
    sigaction(my_sig2, &act, NULL);


    while (receiving);



//    for (int i = 0 ; i < counter ; i++){
//
//
//        if (strcmp(mode, "SIGQUEUE") != 0) {
//            union sigval val;
//            sigqueue(sender_pid, my_sig1, val);
//        } else {
//            kill(sender_pid, my_sig1);
//        }
//    }

    if (strcmp(mode, "SIGQUEUE") != 0) {
        union sigval val;
        sigqueue(sender_pid, my_sig2, val);
    } else {
        kill(sender_pid, my_sig2);
    }

    printf("catcher received %d signals\n", counter);

    return 0;
}