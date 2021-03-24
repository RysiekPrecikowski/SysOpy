#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

#define false 0
#define true 1
#define or ||
#define and &&

/*
    Napisz program, który rozpoczynając od katalogu podanego jako pierwszy parametr uruchomienia,
    idąc w głąb drzewa katalogów, znajdzie pliki zawierające łańcuch podany jako drugi
    parametr uruchomienia programu. Przeszukiwanie każdego z podkatalogów powinno odbyć się
    w osobnym procesie potomnym. Wydruk wyniku wyszukiwania poprzedź wypisaniem ścieżki względnej
    od katalogu podanego jako argument uruchomienia oraz numeru PID procesu odpowiedzialnego za
    przeglądanie określonego (pod)katalogu. Przeszukiwanie powinno obejmować pliki tekstowe
    i pomijać pliki binarne/wykonywalne/obiektowe etc. Program jako trzeci parametr powinien przyjmować
    maksymalną głębokość przeszukiwania licząc od katalogu podanego jako pierwszy parametr.
 */

int searchInFile(char *fileName, char *pattern){
    FILE *fp = fopen(fileName, "r");

    if(fp == NULL)
        return -1;

    char tmp[256];

    while(fgets(tmp, 256, fp) != NULL) {
        if((strstr(tmp, pattern)) != NULL)
            return true;
    }

    fclose(fp);
    return false;
}

int isDirDot(char *dirName){
    if (strlen(dirName) > 2)
        return false;
    return strcmp(".", dirName) == 0 or strcmp("..", dirName) == 0;
}

int checkIfTextFile(char *fileName){
    return (strstr(fileName, ".txt")) != NULL;
}

int main(int argc, char* argv[]){
    if (argc != 4) {
        printf("wrong amount of arguments!");
        return -1;
    }
    char *dirName = argv[1];
    char *matchTo = argv[2];
    int MAXdepth = atoi(argv[3]);
    int actDepth = 0;

    char* actualPath;

    DIR *d;
    struct dirent *dir;
    d = opendir(dirName);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(actDepth > MAXdepth)
                break;

            // create new
            int pathLen = strlen(dirName) + strlen(dir->d_name) + 2;
            actualPath = calloc(pathLen, sizeof (char));
            snprintf(actualPath, pathLen, "%s/%s", dirName, dir->d_name);

            //Check if directory
            if (dir->d_type == DT_DIR){
                if(isDirDot(dir->d_name) == true)
                    continue;

                //create new child, children working directory changes to actualPath
                if (fork()== 0){
                    actDepth ++;

                    dirName = actualPath; //dirName changes to current directory for child
                    d = opendir(dirName);
                }
            } else {
                if (checkIfTextFile(dir->d_name) and searchInFile(actualPath, matchTo) == true)
                    printf("%s found in %s by %d\n", matchTo, actualPath, getpid());
            }
        }
        closedir(d);
    }

    // wait for children to end
    int status;
    while (waitpid(-1, &status, 0) > 0){
        if (WEXITSTATUS(status) != 0)
            return -1;
    }

    free(actualPath);
    return 0;
}