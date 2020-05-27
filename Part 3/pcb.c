#include <stdio.h>
#include <stdlib.h>

typedef struct PCB {
	int pid;
	int PC;
	int PC_page, PC_offset, pages_max;
	int pageTable[10];
	struct PCB* next;
}PCB;


 PCB* makePCB(int pid) {
	struct PCB *p = malloc(sizeof(struct PCB));
	p->pid = pid;
	p->PC = 0;
	p->PC_page = 0;
	p->PC_offset = 0;
	p-> pages_max =0;
	for (int i = 0; i < 10; i++) {
		p->pageTable[i] = (-1);
	}
	p->next = NULL;
	return p;
}

