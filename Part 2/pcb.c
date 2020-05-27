#include <stdio.h>
#include <stdlib.h>

typedef struct PCB {
	int PC;
	int start;
	int end;
	struct PCB* next;
}PCB;


 PCB* makePCB(int start, int end) {
	  struct PCB *p = malloc(sizeof(struct PCB));
		p->start = start;
		p->end = end;
		p->PC = start;
		p->next = NULL;
		return p;
}