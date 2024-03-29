﻿#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]) {
    int pdesk[2];
    if (pipe(pdesk) == -1) {
        perror("PIPE");
        return 1;
    }

    switch (fork()) {
        case -1:
            perror("FORK");
            return 2;
        case 0:
            dup2(pdesk[1], STDOUT_FILENO);
            execvp("ls", argv);
            perror("EXECVP ls");
            exit(1);
        default: {
            /* Zamknij deskryptor do zapisu,
             * przekieruj deskryptor deskryptor wejścia standardowego na deskryptor końca do odczytu tego potoku,
             * wykonaj  tr a-z A-Z, w przypadku błędu  obsłuż go i wyjdź, zwracając 3.
            */

            close(pdesk[WRITE]);
            dup2(pdesk[READ], STDIN_FILENO);
            if (execlp("tr", "tr", "a-z", "A-Z", NULL) == -1){
                return 3;
            }


            /* koniec */
        }
    }
    return 0;
}
