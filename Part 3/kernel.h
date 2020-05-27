#ifndef KERNELL_H
#define KERNELL_H
#include "pcb.h"

PCB* head, * tail;

void addToReady(PCB* p);
PCB* myInit(FILE* f);
void scheduler();
void terminate();

#endif