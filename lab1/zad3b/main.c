#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

#include "myLib.h"

#ifdef DYNAMIC
#include <dlfcn.h>
#endif

double calculate_time(clock_t start, clock_t end){
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}


void printTimes(struct tms *tmsStart, struct tms *tmsEnd, clock_t clockStart, clock_t clockEnd){
    printf("real time: %f\n", calculate_time(clockStart, clockEnd));
    printf(" sys time: %f\n", calculate_time(tmsStart->tms_stime, tmsEnd->tms_stime));
    printf("user time: %f\n", calculate_time(tmsStart->tms_utime, tmsEnd->tms_utime));
}


int main(int argc, char* argv[]) {
#ifdef DYNAMIC
    void* handle = dlopen("./libmyLib.so", RTLD_LAZY);
    mainArray* (*createMainArr)(int) = dlsym(handle, "createMainArr");
    void (*deleteMainArray)(mainArray*) = dlsym(handle, "deleteMainArray");
//    int (*addBlockAtIndex)(mainArray*, block*, int) = dlsym(handle, "addBlockAtIndex");
    int (*addBlockToArray)(mainArray*, block*) = dlsym(handle, "addBlockToArray");
    int (*deleteRowAtIndex)(block*, int) = dlsym(handle, "deleteRowAtIndex");
//    void (*freeBlockMemory)(block*) = dlsym(handle, "freeBlockMemory");
    int (*deleteBlockAtIndex)(mainArray*, int) = dlsym(handle, "deleteBlockAtIndex");
//    char* (*readLine)(FILE*) = dlsym(handle, "readLine");
//    block* (*createBlock)(int) = dlsym(handle, "createBlock");
    block* (*readBlockFromFile)(char*) = dlsym(handle, "readBlockFromFile");
//    int (*getRowCount)(block*) = dlsym(handle, "getRowCount");
    char* (*merge)(char*, char*) = dlsym(handle, "merge");
//    void* (*printBlock)(block*) = dlsym(handle, "printBlock");
//    int* (*findFree)(mainArray*) = dlsym(handle, "findFree");
    block* (*getBlockAtIndex)(mainArray*, int) = dlsym(handle, "getBlockAtIndex");
#endif



    struct tms tmsTotalStart, tmsTotalEnd;
//    struct tms tmsStart, tmsEnd;
    clock_t clockTotalStart, clockTotalEnd;
//    clock_t clockStart, clockEnd;

    double sum = 0;

    clockTotalStart = times(&tmsTotalStart);

    int i = 1;
    mainArray* array = NULL;
    while (i + 1 < argc){
        char *command = argv[i];
        char *arg = argv[i+1];
        if(strcmp(command, "create_table") == 0){
            int size = (int) strtol(arg, NULL, 10);

            array = createMainArr(size);
//            printf("Creating array, size = %d\n", size);


        } else if (strcmp(command, "merge_files") == 0){
            char* f1 = strtok(arg, ":");
            char* f2 = strtok(NULL, ":");

            ////merging
//            clockStart = times(&tmsStart);
            char *merged = merge(f1, f2);
//            clockEnd = times(&tmsEnd);

//            printf("Merging times:\n");
//            printTimes(&tmsStart, &tmsEnd, clockStart, clockEnd);
            ////saving to block
//            clockStart = times(&tmsStart);
            block *b = readBlockFromFile(merged);
//            clockEnd = times(&tmsEnd);

//            printf("Saving block to memory times:\n");
//            printTimes(&tmsStart, &tmsEnd, clockStart, clockEnd);

            int j = addBlockToArray(array, b);

            if (j < 0){
                printf("No free block!");
                return -1;
            }

//            printf("added merged %s and %s at index: %d\n", f1, f2, j);


        } else if (strcmp(command, "remove_block") == 0){
            int index = (int) strtol(arg, NULL, 10);

            ////deleting
//            clockStart = times(&tmsStart);
            int deleted = deleteBlockAtIndex(array, index);
//            clockEnd = times(&tmsEnd);

//            printf("Removing block times:\n");
//            printTimes(&tmsStart, &tmsEnd, clockStart, clockEnd);

            if (deleted < 0){
                printf("failed to remove block");
                return -1;
            }
//            printf("removed block from index %d\n", index);


        } else if (strcmp(command, "remove_row") == 0){
            if (i + 2 >= argc){
                printf("end of arguments!");
                return -1;
            }

            int blockIndex = (int) strtol(arg, NULL, 10);
            int rowIndex = (int) strtol(argv[i+2], NULL, 10);

            block *b = getBlockAtIndex(array, blockIndex);

            if (b == NULL){
                printf("No block at %d", blockIndex);
                return -1;
            }

            ////deleting row
//            clockStart = times(&tmsStart);
            int deletedRow = deleteRowAtIndex(b, rowIndex);
//            clockEnd = times(&tmsEnd);

//            printf("Removing row times:\n");
//            printTimes(&tmsStart, &tmsEnd, clockStart, clockEnd);

            if (deletedRow < 0){
                printf("cannot delete %d row at %d index", rowIndex, blockIndex);
                return -1;
            }
//            printf("deleted %d row at %d index\n", rowIndex, blockIndex);

            i+=1;
        }

        
        i+=2;
//        printf("\n");
    }
    clockTotalEnd = times(&tmsTotalEnd);
    printf("Total times:\n");
    printTimes(&tmsTotalStart, &tmsTotalEnd, clockTotalStart, clockTotalEnd);

    deleteMainArray(array);
    return 0;
}


