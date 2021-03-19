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
Napisz funkcję, która jako parametry pobiera nazwę pliku do odczytu,
 nazwę pliku do zapisu oraz 2 napisy,n1 oraz n2(tablice znaków).
 Zadaniem funkcji jest przepisanie pliku wejściowego do wyjściowego
 w taki sposób, że każde wystąpienie napisu n1 w pliku wejściowym ma
 zostać zamienione na napis n2 w pliku wyjściowym.
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


char *findAndReplace(char *line, char *from, char *to){
    int counter = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (strstr(&line[i], from) == &line[i]) {
            counter++;
            i += strlen(from) - 1;
        }
    }

    char *res = calloc(sizeof (char ), strlen(line )+ counter*(strlen(to) - strlen(from) + 1));

    int i = 0;

    while (*line){
        if (strstr(line, from) == line){
            strcpy(&res[i], to);

            i+= strlen(to);
            line += strlen(from);
        } else{
            res[i++] = *(line++);
        }
    }
    return res;
}

int changeOccurrencesLib(char *in, char *out, char *from, char *to){
    FILE *fp_in = fopen(in, "r");
    FILE *fp_out = fopen(out, "w");

    if (fp_in == NULL or fp_out == NULL)
        return -1;

    char *l1;
    while ((l1 = readLineLib(fp_in)) != NULL) {
        l1 = findAndReplace(l1, from, to);

        fwrite(l1, sizeof (char ), strlen(l1), fp_out);
    }
    return 0;
}

int changeOccurrencesSys(char *in, char *out, char *from, char *to){
    int fd_in = open(in, O_RDONLY);
    int fd_out = open(out, O_WRONLY|O_CREAT|O_TRUNC ,S_IRUSR|S_IWUSR);

    char *l1;
    while ((l1 = readLineSys(fd_in)) != NULL) {
        l1 = findAndReplace(l1, from, to);

        write(fd_out, l1, strlen(l1));
    }
    return 0;

}




int main (int argc, char* argv[]){
    char *in, *out, *from, *to;
    if (argc !=  5)
        return -1;
    in = argv[1];
    out = argv[2];
    from = argv[3];
    to = argv[4];

    Times lib, sys;


    start(&lib);

    changeOccurrencesLib(in, out, from, to);
    end(&lib);



    start(&sys);
    changeOccurrencesSys(in, out, from, to);

    end(&sys);

    printAllStatistics(&lib, &sys, "pomiar_zad_4.txt");

    return 0;
}
