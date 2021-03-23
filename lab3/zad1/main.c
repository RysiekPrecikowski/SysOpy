#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Napisz program, w którym z procesu macierzystego stworzysz
 * dokładnie n procesów potomnych (n parametr wywołania programu).
 * W każdym z procesów potomnych wypisz komunikat, w którym poinformujesz
 * z którego procesu napis pochodzi.
 */

int main(void){
//
//    int n = 5;
//    pid_t child_pid[n];
//    printf("PID glownego programu: %d\n", (int)getpid());
//
//    for(int i = 0 ; i < n ; i++) {
//        child_pid[i] = fork();
//        if (child_pid[i] == 0){
//            printf("Proces dziecko nr %d PID %d rodzic %d\n", i, (int)getpid(), (int)getppid());
////            return 0;
//            break;
//        } else if (child_pid[i] > 0){
////            printf("%d\n", getpid());
//
//            /*
//             * u rodzica czekamy az sie skonczy proces dziecka, inaczej staja sie sierotami :(
//             * i wtedy dostaja inny ppid
//             */
//
//            int status;
//            waitpid(child_pid[i], &status, 0);
//            if (WIFEXITED(status)){
//                int exit_status = WEXITSTATUS(status);
//                printf("dziecko nr %d zakonczylo sie, status %d\n", i, exit_status);
//                if(exit_status != 0){
//                    return exit_status;
//                }
//            }
////            wait(NULL); to jest latwiejsza opcja, teraz nie sprawdzam jak skonczyl sie proces dziecka
//        } else {
//            return -1; //jesli childpid < 0 to mamy errora
//        }
//    }

//    pid_t old_child_pid;
//    printf("PID glownego programu: %d\n", (int)getpid());
//    old_child_pid = fork();
//    if(old_child_pid!=0) {
//        printf("Proces rodzica: Proces rodzica ma pid:%d\n", (int)getpid());
//        printf("Proces rodzica: Proces dziecka ma pid:%d\n", (int)old_child_pid);
//    } else {
//        printf("Proces dziecka: Proces rodzica ma pid:%d\n",(int)getppid());
//        printf("Proces dziecka: Proces dziecka ma pid:%d\n",(int)getpid());
//    }

    pid_t parent_pid = getpid();
    printf("parent pid: %d\n\n", parent_pid);
    int n = 10;
    int myid = 0;
    int status;

    for (int i = 0 ; i < n ; i++){
        if (getpid() == parent_pid){
            myid++;
            fork();
            if (getpid() != parent_pid)
                printf("odpalil sie %d a jego parent to %d\n", getpid(), getppid());
        }
    }

    // jesli to dziecko to wykonuje kod przeznaczony dla dziecka
    if (getpid() != parent_pid)
        printf("   printuje %d a    rodzic   to %d\n", getpid(), getppid());


    // jesli to parent to
    if(getpid() == parent_pid) {
        // If the current process have no child processes, wait(NULL) returns -1 <---- czyli czekam az wszystkie odpalone dzieci umra
//        while (wait(NULL) > 0){
//            printf("czekam se\n");
//        }
        //waitpid (-1, status, 0) dziala jak wait(null), ale dodatkowo mozna zdobyc kod zakonczenia procesu dziecka
        while (waitpid(-1, &status, 0) > 0){
            printf("czekam se, dziecko zwrocilo %d\n", WEXITSTATUS(status));
        }

        printf("\nPARENT UMAR returnuje %d\n\n", getpid());
    } else {
        // my id jest z petli przed sforkowaniem, kazde dziecko ma wlasny numer ktory zwraca do parenta
        printf("\nDZIECKO %d UMARLO\n", getpid());
        return myid;
    }

    return 0;
}