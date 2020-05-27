#ifndef RAM_H
#define RAM_H
#include <stdio.h>

char* ram[40];
int findCell();
void addToRAM(int pageNumber, FILE* f, int frameNumber);
void deleteRAM(int start, int end);
#endif