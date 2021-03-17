#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


char* readLine(FILE* fp){
    if (fp == NULL)
        return NULL;
    size_t maxN = 125;
    char *buff = calloc(maxN, sizeof (char));

    if (fgets(buff, maxN, fp) == NULL)
        return NULL;

    if (feof(fp)){
        char *ptr = buff;
        while(*ptr && *ptr != '\n')
            ptr++;
        *ptr = '\n';
    }

    return buff;
}

int main (int argc, char* argv[])
{
    char *f1, *f2;

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

    FILE *fp1, *fp2;

    fp1 = fopen(f1, "r");
    fp2 = fopen(f2, "r");

    if(fp1 == NULL || fp2 == NULL){
        return -1;
    }

    char *line1, *line2;
    FILE *notEnded = NULL;
    while (true){
        line1 = readLine(fp1);
        line2 = readLine(fp2);
        if (line1 == NULL || line2 == NULL) {
            if (line1 == NULL && line2 == NULL)
                break;

            notEnded = (line1 != NULL) ? fp1 : fp2;
            printf("%s", (line1 != NULL) ? line1 : line2);
            break;
        }
        printf("%s%s", line1, line2);
    }

    while (true){
        line1 = readLine(notEnded);
        if (line1 == NULL)
            break;
        printf("%s", line1);
    }

    fclose(fp1);
    fclose(fp2);

    return 0;
}