#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/times.h>
#include "measureTimes.c"


#define not !
#define and &&
#define or ||

/*
Napisz program, który kopiując podany plik (parametr programu) do
 innego pliku (drugi parametr programu), ”łamie” wiersze, które mają więcej niż 50
 znaków (łącznie ze spacjami). Znaki po-wyżej 50-tego przenoszone są do nowej linii
 (dodatkowy wiersz). Wiersze krótsze kopiowane są bez zmian
 */


char* readLineLib(FILE* fp){
    if (fp == NULL)
        return NULL;
    char *s, c;
    s = calloc(sizeof (char ), 256);
    size_t n;

    for (int i = 0 ; i < 256 ; i++){
        n = fread(&c, sizeof (char), 1, fp);

        if (n == 1){
            s[i] = c;
            if (c == '\n'){
                return s;
            }
        } else
            break;
    }

    return NULL;
}

char* readLineSys(int fd){
    char *s, c;

    s = calloc(sizeof (char ), 256);
    size_t n;


    for(int i = 0 ; i < 256 ; i++){
        n = read(fd, &c, 1);

        if (n == 1){
            s[i] = c;

            if (c == '\n'){
                return s;
            }
        } else
            break;
    }
    return NULL;
}


char *addNewLinesCharacters(char *line, int amount){
    int counter = strlen(line) / amount;

    char *res = calloc(sizeof (char ), strlen(line) + counter + 1);

    int i = 0;

    while (*line){
        if (i % amount == 0 and i >= amount){
            res[i] = '\n';
            i++;
        } else
            res[i++] = *(line++);
    }
    return res;
}

int addNewLinesLib(char *in, char *out, int amount){
    FILE *fp_in = fopen(in, "r");
    FILE *fp_out = fopen(out, "w");

    if (fp_in == NULL or fp_out == NULL)
        return -1;

    char *l1;
    while ((l1 = readLineLib(fp_in)) != NULL) {
        l1 = addNewLinesCharacters(l1, amount);

        fwrite(l1, sizeof (char ), strlen(l1), fp_out);
    }
    return 0;
}

int addNewLinesSys(char *in, char *out, int amount){
    int fd_in = open(in, O_RDONLY);
    int fd_out = open(out, O_WRONLY|O_CREAT|O_TRUNC ,S_IRUSR|S_IWUSR);

    char *l1;
    while ((l1 = readLineSys(fd_in)) != NULL) {
        l1 = addNewLinesCharacters(l1, amount);

        write(fd_out, l1, strlen(l1));
    }
    return 0;
}




int main (int argc, char* argv[]){
    char *in, *out;
    if (argc !=  3)
        return -1;
    in = argv[1];
    out = argv[2];

    int amount = 50;


    Times lib, sys;

    addNewLinesLib(in, out, amount);



    start(&lib);
    addNewLinesLib(in, out, amount);
    end(&lib);


    start(&sys);
    addNewLinesSys(in, out, amount);
    end(&sys);

    printAllStatistics(&lib, &sys, "pomiar_zad_5.txt");
    return 0;
}
