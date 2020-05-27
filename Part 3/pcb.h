#ifndef PCB_H
#define PCB_H
#include <stdio.h>

typedef struct PCB {
    int pid;
    int PC;
    int PC_page, PC_offset, pages_max;
    int pageTable[10];
    struct PCB* next;
}PCB;

PCB* makePCB(int pid);
#endif