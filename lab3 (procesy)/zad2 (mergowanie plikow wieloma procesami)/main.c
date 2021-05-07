#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "measureTimes.c"

#define true 1

/*
    Korzystając z biblioteki z zadania 1 zestawu 1 napisz program do
    mergowania n par plików  w taki sposób, iż każda para plików mergowana
    jest w osobnym procesie potomnym. Przeprowadź pomiary czasu operacji mergowania
    analogiczne do tych z zadania 2 zestawu pierwszego,
    a zatem  dla różnej ilości par (mała, średnia oraz duża ilość par) dla
    różnego stopnia wielkości wierszy w plikach w parze (pliki z dużą ilością wierszy,
    pliki ze średnią ilością wierszy, pliki z małą ilością wierszy). P
    orównaj otrzymane wyniki z tymi uzyskanymi przy okazji rozwiązywania zadań z zestawu pierwszego.
 */

char* readLine(FILE *fp){
    if (fp == NULL)
        return NULL;
    size_t maxN = 125;
    char *buff = calloc(maxN, sizeof (char));
    char* r;
    r = fgets(buff, maxN, fp);
    if (r == NULL)
        return NULL;

    if (feof(fp)){
        char *ptr = buff;
        while(*ptr && *ptr != '\n')
            ptr++;
        *ptr = '\n';
    }
    return buff;
}

int merge(char* fileName1, char* fileName2){
    FILE *f1 = fopen(fileName1, "r");
    FILE *f2 = fopen(fileName2, "r");
    if (f1 == NULL || f2 == NULL)
        return -1;

    char *line1, *line2;
    char resName[20];

    //create tmp file for every child :)
    snprintf(resName, 20, "merged%d.tmp", getpid());
    FILE* fp = fopen(resName,"w");
    if (fp == NULL)
        return -1;

    while (true) {
        line1 = readLine(f1);
        line2 = readLine(f2);
        if (line1 == NULL || line2 == NULL)
            break;
        if (fprintf(fp, "%s%s", line1, line2) < 0)
            return -1;
    }
    fclose(f1);
    fclose(f2);
    fclose(fp);
    return 0;
}


int main(int argc, char* argv[]){
    int i = 1;
    int status;
    pid_t parent_pid = getpid();

    char *arg;
    Times times;

    start(&times);
    while (i + 1 < argc){
        char *command = argv[i];
        arg = argv[i+1];
        if (strcmp(command, "merge_files") == 0){
            if(getpid() == parent_pid){
                // create child process, leave loop with 2 files to merge remembered in arg
                // parent continues going, only child is breaking
                if (fork() == 0)
                    break;
            }
        }
        i+=2;
    }

    if (getpid() != parent_pid) {
        char *f1 = strtok(arg, ":");
        char *f2 = strtok(NULL, ":");

        if (merge(f1, f2) != 0)
            return -1;
    }

    //wait for all children to end
    if(getpid() == parent_pid){
        while (waitpid(-1, &status, 0) > 0){
            if (WEXITSTATUS(status) != 0)
                return -1;
        }

        end(&times);

        printTimes(&times);
    }
    return 0;
}