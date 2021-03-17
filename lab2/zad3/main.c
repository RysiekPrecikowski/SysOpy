#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

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

int main (void)
{
    FILE *fp = fopen("dane.txt", "r");
    FILE *even = fopen("a.txt", "w");
    FILE *tensDigits = fopen("b.txt", "w");
    FILE *square = fopen("c.txt", "w");

    if (fp == NULL || even == NULL
        || tensDigits == NULL || square == NULL)
        return -1;

    int num;
    int evenCounter = 0;
    while (!feof(fp)){
        if (fscanf(fp, "%d\n", &num) != 1)
            return -1;

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
