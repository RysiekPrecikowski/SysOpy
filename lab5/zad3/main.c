#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

#include "utils.h"


char* empty_line(char* buff){
    for_i(LINEMAXLEN)
        buff[i] = blank;
    buff[LINEMAXLEN-1] = '\n';
    return buff;
}

void prepare_out(int n, char* filename){
    FILE* fp = fopen(filename, "w");
    char *line = calloc(LINEMAXLEN, sizeof(char));
    empty_line(line);

    for_i(n){
        if(i == n-1)
            line[LINEMAXLEN - 1] = '\0';
        fprintf(fp, "%s", line);
    }
    fclose(fp);
    free(line);
}

int main(int argc, char* argv[]){
    char consumer_out_path[] = "out.txt";
    int producer_n = 3;

    char pipe_path[] = "pipe";
    mkfifo(pipe_path, 0666);

    int consumer_read = INTLEN + producer_n + 3 + 1;

    for_i(3)
        exec("./consumer", pipe_path, consumer_out_path, int_to_string(consumer_read));

    sleep_seconds(0.4);

    prepare_out(8, consumer_out_path);
    exec("./producer", pipe_path, int_to_string(0), "1", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(1), "0", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(2), "A", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(3), "3", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(4), "2", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(5), "3", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(6), "A", int_to_string(producer_n));
    exec("./producer", pipe_path, int_to_string(7), "2", int_to_string(producer_n));

    wait_all_children;
    remove(pipe_path);
    return 0;
}