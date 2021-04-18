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
    int producer_n = 1;
    int amount_of_consumers = 1;
    int amount_of_producers = 1;
    char consumer_out_path[] = "out.txt";
    char pipe_path[] = "pipe";
    int consumer_read = INTLEN + producer_n + 3 + 1;
    bool prepare = true;

    if (argc == 1){
        mkfifo(pipe_path, 0666);
        exec("./consumer", pipe_path, consumer_out_path, int_to_string(consumer_read));
        prepare_out(amount_of_producers, consumer_out_path);
        exec("./producer", pipe_path, int_to_string(0), "0", int_to_string(producer_n));
    }

    char *arg;
    for (int i = 1 ; i < argc ; i++){
        arg = argv[i];
        if (equals(arg, "producer_n")){
            i+=1;
            producer_n = string_to_int(argv[i]);
            consumer_read = INTLEN + producer_n + 3 + 1;
        } else if (equals(arg, "amount_of_consumers")){
            i += 1;
            amount_of_consumers = string_to_int(argv[i]);
            for_i(amount_of_consumers) {
                exec("./consumer", pipe_path, consumer_out_path, int_to_string(consumer_read));
            }
        } else if (equals(arg, "amount_of_producers")){
            i+=1;
            amount_of_producers = string_to_int(argv[i]);
            if(prepare)
                prepare_out(amount_of_producers, consumer_out_path);
            int j;
            for___(amount_of_producers, j){
                char *producer_path = argv[i + j + 1];
                exec("./producer", pipe_path, int_to_string(j), producer_path, int_to_string(producer_n));
            }
            i += j;
        } else if (equals(arg, "mkfifo")){
            mkfifo(pipe_path, 0666);
        } else if (equals(arg, "not_prepare"))
            prepare = false;
    }

    wait_all_children;
    remove(pipe_path);
    return 0;
}