#ifndef RAM_H
#define RAM_H
#include <stdio.h>


char* ram[1000];
void addToRAM(FILE* f, int* start, int* end);
void removeCommand(int start, int end);

#endif