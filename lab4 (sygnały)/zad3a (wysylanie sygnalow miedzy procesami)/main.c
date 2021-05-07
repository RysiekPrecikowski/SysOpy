#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>
#include <sys/times.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){ // "./main", num_sigs, mode
    pid_t catcher;
    if ((catcher = fork()) == 0){
        execl("./catcher", "./catcher", argv[2], NULL);
    }


    char catcher_pid[10];
    sprintf(catcher_pid, "%d", catcher);
    pid_t sender;

    if ((sender = fork()) == 0){
        execl("./sender", "./sender", argv[1], catcher_pid, argv[2], NULL);
    }

    waitpid(catcher, NULL, 0);
    waitpid(sender, NULL, 0);

    return 0;
}