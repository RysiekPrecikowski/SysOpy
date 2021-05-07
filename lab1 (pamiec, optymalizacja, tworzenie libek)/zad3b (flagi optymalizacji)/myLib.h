
#ifndef LAB1_MYLIB_H
#define LAB1_MYLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define true 1

typedef struct block{
    int rowCount;
    char **rows;
}block;

typedef struct mainArray{
    int blockCount;
    block **blocks;
} mainArray;

mainArray* createMainArr(int arraySize);

int addBlockAtIndex(mainArray* arr, block *b, int ind);

int addBlockToArray(mainArray* arr, block* b);

void deleteMainArray(mainArray* arr);

int deleteRowAtIndex(block* b, int ind);

void freeBlockMemory(block* b);

int deleteBlockAtIndex(mainArray* arr, int ind);

char* readLine(FILE *fp);

block* createBlock(int rowCount);

block* readBlockFromFile(char* fileName);

int getRowCount(block* b);

char* merge(char* fileName1, char* fileName2);

void printBlock(block *b);

int findFree(mainArray *arr);

block* getBlockAtIndex(mainArray* arr, int ind);

#endif //LAB1_MYLIB_H
