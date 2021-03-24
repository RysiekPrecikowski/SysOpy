#include "myLib.h"

mainArray* createMainArr(int arraySize){
    if (arraySize < 0)
        return NULL;

    mainArray *res = malloc(sizeof (mainArray));
    res->blockCount = arraySize;
    res->blocks = calloc(arraySize, sizeof (res->blocks[0]));
    return res;
}

void deleteMainArray(mainArray* arr){
    if (arr == NULL)
        return;
    if (arr->blocks != NULL){
        for (int i = 0 ; i < arr->blockCount ; i++){
            deleteBlockAtIndex(arr, i);
        }
        free(arr->blocks);
        arr->blocks = NULL;
    }
    free(arr);
}

int addBlockAtIndex(mainArray* arr, block *b, int ind){
    if (arr == NULL || ind < 0 || ind >= arr->blockCount)
        return -1;

    arr->blocks[ind] = b;
    return ind;
}

int addBlockToArray(mainArray* arr, block* b){
    if (arr == NULL || arr->blocks == NULL || b == NULL)
        return -1;
    int ind = findFree(arr);

    if (ind < 0)
        return -1;

    return addBlockAtIndex(arr, b, ind);
}

int deleteRowAtIndex(block* b, int ind){
    if(b == NULL || b->rows == NULL || ind <0 ||
       ind >= b->rowCount || b->rows[ind] == NULL)
        return -1;

    free(b->rows[ind]);
    b->rows[ind] = NULL;
    return 0;
}

void freeBlockMemory(block* b){
    if (b == NULL)
        return;

    if (b->rows != NULL){
        for (int i = 0 ; i < b->rowCount ; i++){
            deleteRowAtIndex(b, i);
        }
        free(b->rows);
    }
    free(b);
    b->rows = NULL;
}

int deleteBlockAtIndex(mainArray* arr, int ind){
    if (arr == NULL || arr->blocks == NULL || ind < 0 ||
        ind >= arr->blockCount  || arr->blocks[ind] == NULL)
        return -1;
    freeBlockMemory(arr->blocks[ind]);

    arr->blocks[ind] = NULL;
    return 0;
}

char* readLine(FILE *fp){
    if (fp == NULL)
        return NULL;
    size_t maxN = 125;
    char *buff = calloc(maxN, sizeof (char));
    char* r;
    r = fgets(buff, maxN, fp);
    if (r == NULL)
        return NULL;

    if (feof(fp)){
        char *ptr = buff;
        while(*ptr && *ptr != '\n')
            ptr++;
        *ptr = '\n';
    }
    return buff;
}

block* createBlock(int rowCount){
    if (rowCount < 0)
        return NULL;

    block *res = malloc(sizeof (block));
    res->rowCount = rowCount;
    res->rows = calloc(rowCount, sizeof (res->rows[0]));
    return res;
}

block* readBlockFromFile(char* fileName) {
    int numberOfLines = 0;
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
        return NULL;

    while (readLine(fp))
        numberOfLines += 1;

    block *res = createBlock(numberOfLines);

    rewind(fp);
    for (int i = 0; i < res->rowCount; i++) {
        res->rows[i] = readLine(fp);
    }

    fclose(fp);
    return res;
}

int getRowCount(block* b){
    return b->rowCount;
}

char* merge(char* fileName1, char* fileName2){
    FILE *f1 = fopen(fileName1, "r");
    FILE *f2 = fopen(fileName2, "r");
    if (f1 == NULL || f2 == NULL)
        return NULL;

    char *line1, *line2;
    char *resName = "merged.tmp";
    FILE* fp = fopen(resName,"w");
    if (fp == NULL)
        return NULL;

    while (true) {
        line1 = readLine(f1);
        line2 = readLine(f2);
        if (line1 == NULL || line2 == NULL)
            break;
        fprintf(fp, "%s%s", line1, line2);
    }
    fclose(f1);
    fclose(f2);
    fclose(fp);
    return resName;
}

void printBlock(block *b){
    if (b == NULL || b->rows == NULL || b->rowCount == 0)
        return;

    for (int i = 0 ; i < b->rowCount ; i++){
        if(b->rows[i] != NULL){
            printf("%s", b->rows[i]);
        }
    }
    printf("\n");
}

void printMainArray(mainArray *arr){
    if (arr == NULL || arr->blocks == NULL || arr->blockCount == 0)
        return;

    for (int i = 0 ; i < arr->blockCount ; i++){
        if(arr->blocks[i] != NULL){
            printBlock(arr->blocks[i]);
        }
    }
}

int findFree(mainArray *arr) {
    if (arr == NULL || arr->blockCount == 0)
        return -1;

    for (int i = 0 ; i < arr->blockCount ; i++){
        if (arr->blocks[i] == NULL)
            return i;
    }
    return -1;
}

block *getBlockAtIndex(mainArray *arr, int ind) {
    if (arr == NULL || ind < 0 || ind >= arr->blockCount)
        return NULL;
    return arr->blocks[ind];
}
