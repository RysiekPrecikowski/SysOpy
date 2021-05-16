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
#include <sys/time.h>

#include <pthread.h>
#include "utils.h"


enum MODE {NUMBERS, BLOCK};

short mode;
unsigned short** image;
unsigned short** negative;
int n;
unsigned short range;
int width, height;

typedef struct {
    int i;
    int numbers_from;
    int numbers_to;

    long unsigned res;
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

    long unsigned time;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    switch (mode) {
        case NUMBERS:{
            for___(height, i){
                for___(width, j){
                    if (info->numbers_from <= image[i][j] and image[i][j] < info->numbers_to){
                        negative[i][j] = range - image[i][j];
                    }
                }
            }
            break;
        }
        case BLOCK:{
            for___(height, i){
                for (j = info->numbers_from ; j < info->numbers_to ; j++){
                    negative[i][j] = range - image[i][j];
                }
            }
            break;
        }
    }

    gettimeofday(&end_time, NULL);
    time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + end_time.tv_usec - start_time.tv_usec;

    info->res = time;
    pthread_exit(NULL);
}

void run_threads(pthread_t *threads, thread_info* info){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int i;

    for___(n, i){
        pthread_create(&threads[i], NULL, thread_negative, &info[i]);
    }

    for___(n, i){
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    long unsigned time = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
    print("Total time ---> %-10lu [μs]\n", time);

    for___(n, i){
        print("Thread %-3d ---> %-10lu [μs]", i, info[i].res);
    }
}

int main(int argc, char* argv[]){
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
    if (!fp_out){
        eprint("ERROR WHEN OPENING FILE")
    }

    char *magic_number;
    size_t len;
    getline(&magic_number, &len, fp);

    fscanf(fp,"%d %d %hu", &width, &height, &range);
    range += 1;
    int i, j;

    image = calloc(height, sizeof (*image));
    negative = calloc(height, sizeof (*negative));
    for___(height, i){
        image[i] = calloc(sizeof (int), width);
        negative[i] = calloc(sizeof (int), width);
        for___(width, j){
            fscanf(fp, "%hu", &image[i][j]);
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


    run_threads(threads, info);



    fprintf(fp_out,"%s%d %d\n%d\n", magic_number, width, height, range-1);

    for___(height, i) {
        for___(width, j) {
            fprintf(fp_out,"%4hu", negative[i][j]);
        }
        fprintf(fp_out, "\n");
    }


    for___(height, i){
        free(image[i]);
        free(negative[i]);
    }

    free(image);
    free(negative);
    free(info);

    fclose(fp);
    fclose(fp_out);

    return 0;
}