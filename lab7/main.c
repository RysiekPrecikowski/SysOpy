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
//r -1 s 1
#include "utils.h"

/*
    Napisz program  w ktorym stworzysz semafor systemu V, „opuscisz go”,
    zczytasz i wypiszesz na konsole biezace informacje o jego stanie,
    podniesiesz, zczytasz i wypiszesz na konsole biezace informacje o jego stanie,
    usuniesz stworzony zbior semaforow i zakonczysz program.
 */

/*
 * Zmodyfikuj poprzednie w taki sposób ze opuszczanie semafora odbywa się
 * w procesie macierzystym a jego podnoszenie w procesie potomnym
 *
 * Zmodyfikuj poprzednie w taki sposób ze opuszczanie i podnoszenie
 * semaforow odbywa się w dwoch niezaleznych programach
 */
enum mode {NORMAL, CHILD, INDEPENDENT};

char *path = "/tmp";
int id = 'S';
key_t key;
struct sembuf sembuf = {.sem_num = 0, .sem_op = 1, .sem_flg = 0};
int mode;
int sem_id;
int v;

void get_value(){
    v = semctl(sem_id, 0, GETVAL, 0);
    print("current value %d", v);
}

void get_semaphore(){
    sem_id = semget(key, 1, IPC_CREAT | 0600);
    print("sem id %d", sem_id);
}

void set_value(){
    print("setting value %d", semctl(sem_id, 0, SETVAL, 0));
    get_value();
}

void change_value(int num){
    sembuf.sem_op = num;
    semop(sem_id, &sembuf, 1);
    print("changing value, diff: %d", num)
    get_value();
}

void wait_sem_0(){
    print("Waiting")
    sembuf.sem_op = 0;
    semop(sem_id, &sembuf, 1);
    print("End of waiting");
}


int main(void){
    key = ftok(path, id);

    mode = CHILD;

    switch (mode) {
        case NORMAL:
            get_semaphore();
            set_value();
            change_value(1);
            break;
        case CHILD:
            get_semaphore();
            set_value();
            if (fork() == 0){
                print("CHILD")
                change_value(1);
                sleep(3);
                change_value(-1);
                exit(0);
            } else {
                print("PARENT:")
                get_value();
//                wait_all_children
                sleep(1);
                get_value();
                wait_sem_0();
            }
            break;

        case INDEPENDENT:
            break;
    }





    semctl(sem_id, 0, IPC_RMID);
    return 0;
}