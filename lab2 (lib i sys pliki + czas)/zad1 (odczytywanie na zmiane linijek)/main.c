#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

#include "measureTimes.c"
/*
 Zadanie 1 (20%) Napisz program, który otwiera dwa pliki o nazwach podanych w wierszu poleceń.
 Jeśli argumentów nie podano, wówczas nazwy plików mają być pobrane od użytkownika.
 Program powinien wyświetlać wiersze z obu plików naprzemienne,
 to znaczy: 1-szą linię z pierwszego pliku, 1-szą linię z drugiego pliku, 2-gą linię z pierwszego pliku,
 2-gą linię z drugiego pliku, itd., aż do momentu,
 wyświetlenia ostatniego wiersza pliku zawierającego większą liczbę wierszy.
 */

#define not !
#define and &&
#define or ||



bool readLineLib(FILE* fp){
    if (fp == NULL)
        return NULL;
    char c;

    while(fread(&c, sizeof (char), 1, fp) == 1){
        printf("%c", c);
        if(c == '\n')
            return true;
    }
    return false;
}

bool readLineSys(int fd){
    char c;

    while (read(fd, &c, 1) == 1){
        printf("%c", c);
        if (c == '\n')
            return true;
    }
    return false;
}

int printLinesLib(char* f1, char* f2){
    FILE *fp1, *fp2;

    fp1 = fopen(f1, "r");
    fp2 = fopen(f2, "r");

    if(fp1 == NULL or fp2 == NULL){
        return -1;
    }

    bool l1, l2;
    FILE *notEnded = NULL;
    while (true){
        l1 = readLineLib(fp1);
        l2 = readLineLib(fp2);

        if (not l1 or not l2){
            if (not l1 and not l2){
                break;
            }
            notEnded = (l1) ? fp1 : fp2;
            break;
        }
    }
    while (true){
        l1 = readLineLib(notEnded);
        if (not l1)
            break;
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}


int printLinesSys(char* f1, char* f2){
    int fd1, fd2;
    fd1 = open(f1, O_RDONLY);
    fd2 = open(f2, O_RDONLY);

    if (fd1 < 0 or fd2 < 0)
        return -1;
    bool l1, l2;
    int notEnded = -1;
    while (true){
        l1 = readLineSys(fd1);
        l2 = readLineSys(fd2);

        if (not l1 or not l2){
            if (not l1 and not l2){
                break;
            }
            notEnded = (l1) ? fd1 : fd2;
            break;
        }
    }
    while (true){
        l1 = readLineSys(notEnded);
        if (not l1)
            break;
    }
    return 0;
}


int main (int argc, char* argv[])
{
    char *f1 = NULL, *f2 = NULL;

    if(argc == 1){
        char in1[50], in2[50];
        printf("enter file names\n");
        scanf("%s %s", in1, in2);
        f1 = in1;
        f2 = in2;
    } else {
        if (argc == 3){
            f1 = argv[1];
            f2 = argv[2];
        }
    }

    Times lib, sys;

    printf("\nLib\n\n");

    start(&lib);
    if(printLinesLib(f1, f2) < 0)
        return -1;

    end(&lib);



    printf("\n\nSys\n\n");
    start(&sys);
    if(printLinesSys(f1, f2) < 0)
        return -1;
    end(&sys);

    printAllStatistics(&lib, &sys, "pomiar_zad_1.txt");

    return 0;
}