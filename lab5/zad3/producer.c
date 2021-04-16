#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

char* to_send(char* read, int row, char* buff){
    memset(buff,0,strlen(buff));
    sprintf(buff, "%d-:-%s", row, read);
    return buff;
}

int main(int argc, char* argv[]){
    check_args(5);

    char *pipe_path = argv[1];
    int row = string_to_int(argv[2]);
    char *in = argv[3];
    int n = string_to_int(argv[4]);

    char buff[n];

    FILE *pipe = fopen(pipe_path, "w");
    check_null(pipe);

    FILE *in_fp = fopen(in, "r");
    check_null(in_fp);

    int sent_size = INTLEN + n + 3 + 1;
    char *to_send_buff = calloc(sent_size, sizeof (char));

    while (fread(buff, sizeof (char ), n, in_fp) > 0){
        to_send(buff, row, to_send_buff);
        fwrite(to_send_buff, sizeof(char), sent_size , pipe);
        fflush(pipe);
        sleep_seconds(0.3);
        memset(buff,0,strlen(buff));
    }

    free(to_send_buff);

    fclose(in_fp);
    fclose(pipe);
    return 0;
}