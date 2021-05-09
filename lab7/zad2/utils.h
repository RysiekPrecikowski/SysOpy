#ifndef SYSOPY_UTILS_H
#define SYSOPY_UTILS_H
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

#define and &&
#define or ||
#define not !
#define is ==

#define string char*

#define dunno "¯\\_(ツ)_/¯"

#define print(args...) {printf(args); printf("\n"); fflush(stdout);}
#define eprint(args...) {fprintf(stderr, args); fprintf(stderr, "\n"); fflush(stderr);fflush(stdout);}

#define my_exit(code) {print("EXITING WITH CODE %d", code); exit(code);}

#define for_i(n) for(int i = 0 ; i < n ; i++)
#define for___(n, i) for(i = 0 ; i < n ; i++)
#define for_el_in(el, arr, n) for(int i = 0, el = arr[i] ; i < n ; el = arr[++i])
#define for_el_in_it(el, arr, n, it) for(it = 0, el = arr[i] ; it < n ; arr[++it])

#define check_args(n) if (argc != n) exit(-1)

#define check_null(pointer) if (pointer == NULL) exit(-1)

#define sleep_milliseconds(n) usleep(n * 1000)

#define sleep_seconds(n) usleep(n * 1000 * 1000)



/// PROCESSES
#define exec(program, args...) \
        if(fork() == 0) \
            execl(program, program, args, NULL)

#define exec_pid(pid, program, args...) \
        if((pid = fork()) == 0) \
            execl(program, program, args, NULL)

#define wait_all_children while(wait(NULL) > 0)




/// STRING
#define INTLEN 12

#define equals(s1, s2) strcmp(s1, s2) == 0

char* int_to_string(int n){
    char *str = calloc(INTLEN, sizeof (char ));
    sprintf(str, "%d", n);
    return str;
}

char *copy_string(char *str) {
    char *res = calloc(strlen(str) + 1, sizeof(char));
    strncpy(res, str, strlen(str));
    return res;
}


#define string_to_int(str) atoi(str)


int get_first_int(char *text) {
    int number;
    if (sscanf(text, "%*[^0123456789]%d", &number))
        return number;

    if (sscanf(text, "%d", &number))
        return number;
    return INT_MAX;
}

#define set_array(arr, size, content){for_i(size) arr[i] = content;}

#define print_array(arr, size, format){for_i(size) printf(format, arr[i]); printf("\n");}



void print_current_time(){
//    time_t rawtime;
//    struct tm * timeinfo;
//
//    time ( &rawtime );
//    timeinfo = localtime ( &rawtime );
//    printf ( "CURRENT TIME: %s", asctime (timeinfo) );





//    struct timespec now;
//    timespec_get(&now, TIME_UTC);
//    int64_t mil = ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
//    print("%" PRId64 "\n", mil)



    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

    char currentTime[84] = "";
    sprintf(currentTime, "%s:%03d", buffer, milli);
    printf("current time: %s \n", currentTime);

    print(" ");
}

#endif //SYSOPY_UTILS_H
