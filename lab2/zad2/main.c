#include <stdio.h>
#include <string.h>

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

    char buff[256];

    while (fgets(buff, sizeof (buff), fp)){
        if (strchr(buff, *c) != NULL){
            printf("%s", buff);
        }
    }

    return 0;
}