#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>
#include "utils.h"
#include "measureTimes.c"

enum MODE {NUMBERS, BLOCK};

short mode;
unsigned short** values;
unsigned short** negative;
int n;
unsigned short range;
int width, height;

typedef struct {
    int i;
    int numbers_from;
    int numbers_to;

    int res;
}thread_info;

void prepare_info_numbers(thread_info* info, int n, int range){
    int from, to;
    for_i(n){
        info[i].i = i;

        from = i * range/n;
        to = (i+1) * range/n;

        info[i].numbers_from = from;
        info[i].numbers_to = to;
    }
}



void* thread_negative(void *arg){
    thread_info *info = (thread_info *) arg;

    int i,j;

    switch (mode) {
        case NUMBERS:{
            for___(height, i){
                for___(width, j){
                    if (info->numbers_from <= values[i][j] and values[i][j] < info->numbers_to){
                        negative[i][j] = range - values[i][j];
                    }
                }
            }
            break;
        }
        case BLOCK:{
            for___(height, i){
                for (j = info->numbers_from ; j < info->numbers_to ; j++){
                    negative[i][j] = range - values[i][j];
                }
            }
            break;
        }
    }



    info->res = info->i;
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    Times times;

    char* path = argv[1];

    n = string_to_int(argv[2]);

    if (equals(argv[3], "block")){
        mode = BLOCK;
    }
    if (equals(argv[3], "numbers")){
        mode = NUMBERS;
    }


    FILE *fp = fopen(path, "r");
    if (!fp){
        eprint("ERROR WHEN OPENING FILE")
    }
    FILE *fp_out = fopen("negative", "w");
    if (!fp){
        eprint("ERROR WHEN OPENING FILE")
    }

    char *tmp;
    size_t len;


    getline(&tmp, &len, fp);

    fscanf(fp,"%d %d %hu", &width, &height, &range);
    range += 1;
    int i, j;

    values = calloc(height, sizeof (*values));
    negative = calloc(height, sizeof (*negative));
    for___(height, i){
        values[i] = calloc(sizeof (int), width);
        negative[i] = calloc(sizeof (int), width);
        for___(width, j){
            fscanf(fp, "%hu", &values[i][j]);
        }
    }

    pthread_t threads[n];
    thread_info *info = calloc(n, sizeof (thread_info));

    switch (mode) {
        case NUMBERS:
            prepare_info_numbers(info, n, range);
            break;
        case BLOCK:
            prepare_info_numbers(info, n, width);
            break;
        default: eprint("not recognized mode");
    }

    start(&times);
    for___(n, i){
        pthread_create(&threads[i], NULL, thread_negative, &info[i]);
    }
    sleep_seconds(0.1);


    for___(n, i){
        pthread_join(threads[i], NULL);
    }
    end(&times);

    fprintf(fp_out,"%s%d %d\n%d\n", tmp, width, height, range-1);

    for___(height, i) {
        for___(width, j) {
            fprintf(fp_out,"%4hu", negative[i][j]);
        }
        fprintf(fp_out, "\n");
    }

//    printTimes(&times);

//    //TODO freeeeeeeeeeeeee timesss

    fclose(fp);
    fclose(fp_out);

    return 0;
}