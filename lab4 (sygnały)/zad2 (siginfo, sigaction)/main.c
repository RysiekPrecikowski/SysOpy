#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void handler_SIG(int sig, siginfo_t *info, void *context){
    printf("\nSIGINT handler\n");
    printf("Signal number from sig: %d\n", sig);
    printf("Signal number from siginfo: %d\n", info->si_signo);
    printf("Real user ID of sending process: %d\n", info->si_uid);
    printf("Process ID of sending process: %d\n", info->si_pid);

}

void test_SA_SIGINFO(void) {
    printf("\n************     SIGINFO TEST     *************\n");
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler_SIG;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_flags = 0;

    printf("SIGINFO not set\n");
    raise(SIGINT);

    printf("\n\nSIGINFO set\n");
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &sa, NULL);

    raise(SIGINT);
}

void handler_SIGCHLD(int sig, siginfo_t *info, void *context) {
    printf("handler_SIGCHLD PID %d Child PID %d \n", getpid(), info->si_pid);
}

void send_signal_to_child(int sig){
    pid_t child_id = fork();

    if(child_id == 0) {
        while (1);
    } else {
        sleep(1);
        printf("%d sending %d to %d\n", getpid(), sig, child_id);

        kill(child_id, sig);

        fflush(stdout);
        sleep(1);
    }
}


void test_NOCLDSTOP(void){
    // SA_NOCLDSTOP -> get SIGHCLD only if child is dead
    printf("\n************     NOCLDSTOP TEST     *************\n");
    struct sigaction act = {.sa_flags = SA_SIGINFO, .sa_sigaction = handler_SIGCHLD};
    sigaction(SIGCHLD, &act, NULL);

    printf("\nSA_NOCLDSTOP not set\n\n");

    send_signal_to_child(SIGSTOP);
    send_signal_to_child(SIGKILL);


    fflush(stdout);
    sleep(1);

    printf("\nSA_NOCLDSTOP set\n\n");

    act.sa_flags = SA_NOCLDSTOP | SA_SIGINFO;
    sigaction(SIGCHLD, &act, NULL);

    send_signal_to_child(SIGSTOP);
    send_signal_to_child(SIGKILL);

    printf("\nflaga SA_NOCLDSTOP sprawila, ze parent otrzymuje powiadomienie jedynie o smierci dziecka\n");
}



void test_SA_RESETHAND(void){
    printf("\n************     RESETHAND TEST     *************\n");
    struct sigaction act = {.sa_flags = SA_SIGINFO, .sa_sigaction = handler_SIGCHLD};
    sigaction(SIGCHLD, &act, NULL);

    printf("\nSA_NOCLDSTOP not set\n\n");

    send_signal_to_child(SIGKILL);
    send_signal_to_child(SIGKILL);


    fflush(stdout);
    sleep(1);

    printf("\nRESETHAND set\n\n");

    act.sa_flags = SA_RESETHAND | SA_SIGINFO;
    sigaction(SIGCHLD, &act, NULL);

    send_signal_to_child(SIGKILL);
    send_signal_to_child(SIGKILL);

    printf("flaga RESETHAND sprawila, ze po pierwszym sygnale handler zostal przywrocony do domyslnego\n");
}

int main(void){
    test_SA_SIGINFO();

    test_NOCLDSTOP();

    test_SA_RESETHAND();

    signal(SIGQUIT, SIG_IGN);
    kill(-getpid(), SIGQUIT);
    return 0;
}