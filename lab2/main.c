#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
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
//    char *f;
//    char *c;
//
//    if(argc == 1){
//        char in[50], ch;
//        printf("enter char and file name\n");
//        scanf("%c %s", &ch, in);
//        f = in;
//        c = &ch;
//    } else {
//        if (argc == 3){
//            c = argv[1];
//            f = argv[2];
//        }
//    }

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











//    FILE *fp; /* używamy metody wysokopoziomowej - musimy mieć zatem identyfikator pliku, uwaga na
//gwiazdkę! */
//    char tekst[] = "Hello world";
//    if ((fp=fopen("test.txt", "w"))==NULL) {
//        printf ("Nie mogę otworzyć pliku test.txt do zapisu!\n");
//        exit(1);
//    }
//    fprintf (fp, "%s", tekst); /* zapisz nasz łańcuch w pliku */
//    fclose (fp); /* zamknij plik */
//
//    //////////////////////////////////////////////////////////
//
//    /// funkcje systemowe
//    char c;
//    int in, out;
//    in = open("in", O_RDONLY);
//    out = open("out", O_WRONLY|O_CREAT); //,S_IRUSR|S_IWUSR bez tych flag plik jest tworzony bez zadnych uprawnien
//
//    printf("deskryptor: %d\n\n", in);
//
//    while (read(in, &c, 1) == 1){
//        printf("%c\n", c);
//        write(out, &c, 1);
//    }
//
//    FILE *fp_in = fopen("in", "r");
//    FILE *fp_out = fopen("out", "w");
//
//    while(fread(&c, sizeof (char), 1, fp_in) == 1){
//        printf("%c\n", c);
//        fwrite(&c, sizeof (char ), 1, fp_out);
//    }
//
//    fclose(fp_in);
//    fclose(fp_out);


}
#pragma clang diagnostic pop