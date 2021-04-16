#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

#define and &&

#define for_i(n) for(int i = 0 ; i < n ; i++)
#define for___(n, i) for(i = 0 ; i < n ; i++)

#define MODE_LIST 1
#define MODE_SEND 2
#define LIST_DATE 3
#define LIST_SENDER 4

#define equals(check, input) strcmp(check, input) == 0


void list(int list_mode){
    FILE *fp;
    char path[100];

    if (list_mode == LIST_DATE)
        fp = popen("echo | mail | tail -n +2 | head -n -2 | tac ", "r");
    else if(list_mode == LIST_SENDER)
        fp = popen("echo | mail | tail -n +2 | head -n -2 | sort -u -k2", "r");
    else
        exit(-4);
    while (fgets(path, 100, fp) != NULL)
        printf("%s", path);
}

void send(char* to, char* subject, char* message){
    char command[strlen("mail -s ") + strlen(subject) + strlen(to) + 1];
    FILE *fp;
    sprintf(command, "mail -s %s %s", subject, to);
    fp = popen(command, "w");

    fprintf(fp, "%s", message);
}


int main(int argc, char* argv[]){
    int mode;
    if (argc == 2)
        mode = MODE_LIST;
    else if (argc == 4)
        mode = MODE_SEND;
    else
        return -1;

    if (mode == MODE_LIST){
        if (equals("sender", argv[1]))
            list(LIST_SENDER);
        else if (equals("date", argv[1]))
            list(LIST_DATE);
        else
            return -2;
    }

    if (mode == MODE_SEND){
        send(argv[1], argv[2], argv[3]);
    }

    return 0;
}