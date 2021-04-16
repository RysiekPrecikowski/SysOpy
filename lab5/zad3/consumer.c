#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/file.h>
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include<fcntl.h>
#include<signal.h>
#include<stdbool.h>

#include "utils.h"


void append_line(int line, FILE* fp, char* text) {
    rewind(fp);
    char buff[LINEMAXLEN +1];
    int counter = 0, i;

    flock(fileno(fp), LOCK_EX);

    while (fgets(buff, LINEMAXLEN + 1, fp) != NULL){

        if (counter == line) {
            for___(LINEMAXLEN, i){
                if (buff[i] == blank)
                    break;
            }
            fseek(fp, (counter)*LINEMAXLEN + i, SEEK_SET);
            fputs(text, fp);
            fflush(fp);
            break;
        }
        counter++;
    }
    flock(fileno(fp), LOCK_UN);
}

int main(int argc, char* argv[]){
    check_args(4);

    char *pipe_path = argv[1];
    char *out = argv[2];
    int n = string_to_int(argv[3]);

    char buff[n];

    FILE *pipe_fp = fopen(pipe_path, "r");
    check_null(pipe_fp);

    FILE *out_fp = fopen(out, "r+");

    check_null(out_fp);

    while (fread(buff, sizeof (char ), n , pipe_fp) == n){
        int line = get_first_int(buff);

        strtok(buff, "-:");
        char* pch = strtok(NULL, "-:");

        append_line(line, out_fp, pch);
    }

    fclose(out_fp);
    fclose(pipe_fp);
    return 0;
}