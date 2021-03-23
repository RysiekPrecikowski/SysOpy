#include <sys/times.h>
#include <stdio.h>

typedef struct Times{
    struct tms tmsStart, tmsEnd;
    clock_t clockStart, clockEnd;
} Times;

double calculate_time(clock_t start, clock_t end){
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void printTimes(Times *time){
    printf("real time: %f\n", calculate_time(time->clockStart, time->clockEnd));
    printf(" sys time: %f\n", calculate_time(time->tmsStart.tms_stime, time->tmsEnd.tms_stime));
    printf("user time: %f\n", calculate_time(time->tmsStart.tms_utime, time->tmsEnd.tms_utime));
}

void start(Times *time){
    time->clockStart = times(&time->tmsStart);
}

void end(Times *time){
    time->clockEnd = times(&time->tmsEnd);
}

void printTimesToFile(Times *time, FILE *fp){
    fprintf(fp, "real time: %f\n", calculate_time(time->clockStart, time->clockEnd));
    fprintf(fp, " sys time: %f\n", calculate_time(time->tmsStart.tms_stime, time->tmsEnd.tms_stime));
    fprintf(fp, "user time: %f\n", calculate_time(time->tmsStart.tms_utime, time->tmsEnd.tms_utime));
}


void printAllStatistics(Times *lib, Times *sys, char *filename){
    printf("\nLib times:\n");
    printTimes(lib);

    printf("\nSys times:\n");
    printTimes(sys);

    FILE *resFP = fopen(filename, "w");
    fprintf(resFP, "\nLib times:\n");
    printTimesToFile(lib, resFP);
    fprintf(resFP, "\nSys times:\n");
    printTimesToFile(sys, resFP);
}