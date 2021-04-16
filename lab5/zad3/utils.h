#ifndef SYSOPY_UTILS_H
#define SYSOPY_UTILS_H
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

#define and &&
#define or ||
#define not !


#define print(args...) printf(args); printf("\n");

#define for_i(n) for(int i = 0 ; i < n ; i++)
#define for___(n, i) for(i = 0 ; i < n ; i++)

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

#define string_to_int(n) atoi(n)

int get_first_int(char *text) {
    int number;
    if (sscanf(text, "%*[^0123456789]%d", &number))
        return number;

    if (sscanf(text, "%d", &number))
        return number;
    return INT_MAX;
}

#define blank '_'
#define LINEMAXLEN 128
#endif //SYSOPY_UTILS_H
