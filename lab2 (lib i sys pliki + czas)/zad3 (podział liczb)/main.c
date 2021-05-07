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
 * W pliku dane.txt znajdują się w kolejnych wierszach losowe liczby.
 * Do pliku a.txt wpisz ilość liczb parzystych znajdujących się
 * w pliku dane.txt w następującej postaci: ”Liczb parzystych
 * jest [ilość liczb]”. Do pliku b.txt skopiuj wszystkie liczby
 * z pliku dane.txt, w których cyfra dziesiątek jest równa 7 lub 0.
 * Do pliku c.txt skopiuj wszystkie liczby, które są kwadratami liczb
 * całkowitych, np. taką liczbą jest liczba 225, ponieważ 225 = 15^2.
 */

int MAX_LEN = 10;

bool isSquare(int num){
    int l = 1, r = num;
    while (l <= r){
        int m = (l + r ) / 2;

        if(m * m == num)
            return true;
        if (m*m < num)
            l = m + 1;
        else
            r = m - 1;
    }
    return false;
}

int readNumberLib(FILE* fp, int *num, char* string, int* k){
    *num = 0;
    char c;
    int sign = 1;

    for (int i = 0 ; i < MAX_LEN ; i++)
        string[i] = '\0';
    *k = 0;
    while(fread(&c, sizeof (char), 1, fp) == 1){
        string[*k] = c;
        (*k)++;
        if (c != '\n') {
            if (c == '-')
                sign = -1;
            else {
                *num *= 10;
                *num += c - '0';
            }
        }
        if(c == '\n') {
            *num *= sign;
            return 0;
        }
    }

    return -1;
}


int readNumberSys(int fd, int* num, char* string, int* k){
    *num = 0;
    char c;
    int sign = 1;

    for (int i = 0 ; i < MAX_LEN ; i++)
        string[i] = '\0';

    *k = 0;
    while (read(fd, &c, 1) == 1){
        string[*k] = c;
        (*k)++;
        if (c != '\n') {

            if (c == '-')
                sign = -1;
            else {
                *num *= 10;
                *num += c - '0';
            }
        }
        if(c == '\n') {
            *num *= sign;
            return 0;
        }
    }
    return -1;
}

int categorizeNumbersSys(){
    int fd = open("dane.txt", O_RDONLY);
    int even = open("a.txt", O_WRONLY|O_CREAT|O_TRUNC ,S_IRUSR|S_IWUSR);
    int tensDigits = open("b.txt", O_WRONLY|O_CREAT|O_TRUNC ,S_IRUSR|S_IWUSR);
    int square = open("c.txt", O_WRONLY|O_CREAT|O_TRUNC ,S_IRUSR|S_IWUSR);

    int num;
    int evenCounter = 0;

    char string[MAX_LEN];
    int len;
    while (true){
        if (readNumberSys(fd, &num, string, &len) != 0)
            break;

        if (num % 2 == 0){
            evenCounter += 1;
        }
        int tDigit = (num % 100 - num % 10) / 10;
        if ((tDigit == 7 && num >= 10 )
            || (tDigit == 0 && num >= 100)){

            write(tensDigits, string, len);
        }
        if (isSquare(num)) {
            write(square, string, len);
        }
    }
    char tekst[] = {"Liczb parzystych jest "};
    write(even, tekst, sizeof(tekst) -1);

    char tmp[20];
    for (int i = 0 ; i < sizeof (tmp) ; i++)
        tmp[i] = '\0';
    sprintf(tmp,"%d", evenCounter);

    int k = 0;
    while(evenCounter > 0){
        k+=1;
        evenCounter /=10;
    }

    write(even, tmp, k);

    return 0;
}


int categorizeNumbersLib(){
    FILE *fp = fopen("dane.txt", "r");
    FILE *even = fopen("a.txt", "w");
    FILE *tensDigits = fopen("b.txt", "wb");
    FILE *square = fopen("c.txt", "w");

    if (fp == NULL || even == NULL
        || tensDigits == NULL || square == NULL)
        return -1;

    int num;
    int evenCounter = 0;
    char string[MAX_LEN];
    int len;
    while (true){
        if (readNumberLib(fp, &num, string, &len) != 0)
            break;

//        printf("%d %s %d\n", num, string, len);
        if (num % 2 == 0){
            evenCounter += 1;
        }
        int tDigit = (num % 100 - num % 10) / 10;
        if ((tDigit == 7 && num >= 10 )
            || (tDigit == 0 && num >= 100)){

            fwrite(string, sizeof (char ), len, tensDigits);

        }
        if (isSquare(num)) {
            fwrite(string, sizeof (char ), len, square);
        }
    }
    char tekst[] = {"Liczb parzystych jest "};
    fwrite(tekst, sizeof (char ), sizeof(tekst) -1, even);

    char tmp[20];
    for (int i = 0 ; i < sizeof (tmp) ; i++)
        tmp[i] = '\0';
    sprintf(tmp,"%d", evenCounter);

    int k = 0;
    while(evenCounter > 0){
        k+=1;
        evenCounter /=10;
    }

    fwrite(tmp, sizeof (char ), k, even);

    fclose(fp);
    fclose(even);
    fclose(tensDigits);
    fclose(square);
    return 0;
}



int main (void)
{
    Times lib, sys;

    start(&lib);
    if (categorizeNumbersLib() != 0)
        return -1;
    end(&lib);



    start(&sys);
    if (categorizeNumbersSys() != 0)
        return -1;
    end(&sys);

    printAllStatistics(&lib, &sys, "pomiar_zad_3.txt");


    return 0;
}
