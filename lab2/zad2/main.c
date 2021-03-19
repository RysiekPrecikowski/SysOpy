#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/times.h>
#include "measureTimes.c"
#define not !
#define and &&
#define or ||
/*
 * Napisz program, który przyjmuje 2 argumenty wiersza poleceń.
 * Pierwszy z argumentów jest znakiem, drugi nazwą pliku.
 * Program powinien wyświetlić na ekranie tylko te
 * wiersze pliku wejściowego,które zawierają dany znak.
 * Zakładamy, że każdy wiersz w pliku kończy się znakiem
 * przejścia do nowej linii. Przyjmujemy, że żaden wiersz
 * nie przekracza długości 256 znaków.
 */


bool readLineLib(FILE* fp, char pattern){
    if (fp == NULL)
        return NULL;
    char s[256], c;

    size_t n;
    bool flag = false;
    for (int i = 0 ; i < 256 ; i++){
        n = fread(&c, sizeof (char), 1, fp);

        if (n == 1){
            s[i] = c;

            if (c == pattern)
                flag = true;

            if (c == '\n'){
                if(flag)
                    printf("%s", s);
                return true;
            }
        } else
            break;
    }

    return false;
}

bool readLineSys(int fd, char pattern){
    char s[256], c;

    size_t n;
    bool flag = false;

    for(int i = 0 ; i < 256 ; i++){
        n = read(fd, &c, 1);

        if (n == 1){
            s[i] = c;

            if (c == pattern)
                flag = true;

            if (c == '\n'){
                if (flag)
                    printf("%s", s);
                return true;
            }
        } else
            break;
    }
    return false;
}


int main (int argc, char* argv[])
{
    char *f;
    char *c;

    if(argc == 1){
        char in[50], ch;
        printf("enter char and file name\n");
        scanf("%c %s", &ch, in);
        f = in;
        c = &ch;
    } else {
        if (argc == 3){
            c = argv[1];
            f = argv[2];

        }
    }

    FILE *fp = fopen(f, "r");

    if (fp == NULL)
        return -1;

    Times lib, sys;

    printf("\nLib\n\n");
    start(&lib);
    while(readLineLib(fp, *c));

    end(&lib);


    int fd = open(f, O_RDONLY);
    printf("\n\nSys\n\n");

    start(&sys);
    while(readLineSys(fd, *c));

    end(&sys);

    printAllStatistics(&lib, &sys, "pomiar_zad_2.txt");

    return 0;
}