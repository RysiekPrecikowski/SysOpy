#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void handler_SIGINT(int sig, siginfo_t *info, void *context){
    printf("\nSIGINT handler\n");
    printf("Signal number from sig: %d\n", sig);
    printf("Signal number from siginfo: %d\n", info->si_signo);
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Process ID of sending process: %d\n", info->si_pid);

}

void test_SA_SIGINFO(void) {
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler_SIGINT;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_flags = 0;

    printf("SIGINFO not set\n");
    raise(SIGINT);

    printf("\n\nSIGINFO set\n");
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    raise(SIGINT);
}


int main(void){
//    struct sigaction sa;
//    sigaction(SIGUSR1, 0, &sa);
//    printf("PID: %d ", getpid());
//    if(sa.sa_handler==SIG_IGN)
//        printf("ignored\n");
//    else if(sa.sa_handler == handler)
//        printf("user defined handler\n");
//    else if(sa.sa_handler == SIG_BLOCK)
//        printf("blocked\n");
//    else
//        printf("default ¯\\_(ツ)_/¯\n");

    test_SA_SIGINFO();
    return 0;
}