#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define not !
#define and &&
#define or ||

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

int readNumberLib(FILE* fp, int *num){
    *num = 0;
    char c;
    int sign = 1;

    while(fread(&c, sizeof (char), 1, fp) == 1){

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

int doItSys(){
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


int doItLib(){
    FILE *fp = fopen("dane.txt", "r");
    FILE *even = fopen("a.txt", "w");
    FILE *tensDigits = fopen("b.txt", "wb");
    FILE *square = fopen("c.txt", "w");

    if (fp == NULL || even == NULL
        || tensDigits == NULL || square == NULL)
        return -1;

    int num;
    int evenCounter = 0;

    while (true){
        if (readNumberLib(fp, &num) != 0)
            break;

        printf("%d\n", num);
        if (num % 2 == 0){
            evenCounter += 1;
        }
        int tDigit = (num % 100 - num % 10) / 10;
        if ((tDigit == 7 && num >= 10 )
            || (tDigit == 0 && num >= 100)){

            if(fprintf(tensDigits, "%d\n", num) < 0)
                return -1;

        }
        if (isSquare(num)) {
            if(fprintf(square, "%d\n", num) < 0)
                return -1;
        }
    }
    if (fprintf(even, "Liczb parzystych jest %d", evenCounter) < 0)
        return -1;

    fclose(fp);
    fclose(even);
    fclose(tensDigits);
    fclose(square);
    return 0;
}


int main (void)
{
    if (doItLib() != 0)
        return -1;

    if (doItSys() != 0)
        return -1;

    return 0;
}
