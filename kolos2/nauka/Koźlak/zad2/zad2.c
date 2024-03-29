

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define BUF_SIZE 1000


union semun {                   /* Uzywane do semctl() */
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
    struct seminfo *    __buf;
};

typedef enum { FALSE, TRUE } Boolean;


Boolean bsUseSemUndo = FALSE;
Boolean bsRetryOnEintr = TRUE;


int                     /* Inicjalizacja dostepnego semafora*/
initSemAvailable(int semId, int semNum)
{
    union semun arg;

    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int                     /* Inicjalizacja zajetego semafora */
initSemInUse(int semId, int semNum)
{
    union semun arg;

    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

/* Operacja P*/

int                     
reserveSem(int semId, int semNum)
{
    struct sembuf sops = {.sem_num = semNum, .sem_op = -1, .sem_flg = 0};


    /* Ustaw odpowiednie pola struktury sops i wykonaj 
       operacje na semaforze semid*/

    return semop(semId, &sops, 1);
}

int     /* Operacja V */
releaseSem(int semId, int semNum)
{
    struct sembuf sops = {.sem_num = semNum, .sem_op = 1, .sem_flg = 0};
     /* Ustaw odpowiednie pola struktury sops i wykonaj 
       operacje na semaforze semid*/

    return semop(semId, &sops, 1);
}


int
main(int argc, char *argv[])
{
    int semId=1;
     int s;

     
    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s good OR %s bad\n",argv[0],argv[0]);
        exit(-1);
    }


     semId = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);

    if(strcmp(argv[1], "good") == 0)
    {
     s = initSemAvailable(semId, 0);

     s = reserveSem(semId, 0);
     printf("Sekcja krytyczna! \n");
     s = releaseSem(semId, 0);

    }
    else if(strcmp(argv[1], "bad") == 0) 
    {
     s = initSemInUse(semId, 0);

     s = reserveSem(semId, 0);
     printf("Sekcja krytyczna! \n");
     s = releaseSem(semId, 0);
    }
    else 
     {
        printf("Argument error \n");
        exit(-1); 
     }

    exit(EXIT_SUCCESS);
}
