#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "utils.h"

/*
    Napisz fragment kodu tworzący kolekje komunikatow systemu V, wysylajacy do
    niej komunikat „Hello World”, odczytujący wartość z kolejki do osobnego bufora,
    wypisujący wartość odczytana z kolejki na ekran i usuwający kolejke
 */


typedef struct{
    long mtype;
    char text[256];
}message;

int main(void){
    char *path = "/tmp";
    int id = 'S';
    key_t key = ftok(path, id);

    int q = msgget(key, IPC_CREAT | 0666);

    message mess = {1, "Hello World!"};
    msgsnd(q, &mess, 256, 0);

    message received;
    msgrcv(q, &received, 256, 1, 0);

    print("%s", received.text);

    return 0;
}