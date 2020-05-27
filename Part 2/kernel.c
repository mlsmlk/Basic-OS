#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"

int start=0;
int end=0;
FILE* f;
int count = 0;

PCB* head, * tail;

int main() {
	return shellUI();
}

void addToReady(PCB* p) {
	if (head == NULL && tail == NULL) {
		head = p;
		tail = head;
		count=1;
	}
	else {
		if (head == tail) {
			tail = p;
			head->next = tail;
			count=2;
		}
		else {
			tail = p;
			count=3;
		}
	}
}

int rQNotEmpty() {
	if (head != NULL ) {
		return 1;
	}else {
		return 0;
	}
}

void scheduler() {
	//check the ready queue is completely empty -> exec function ends
	int endOfFile = 0;
	int k = 0;
	while (rQNotEmpty() == 1) {
		if (isAvailable() == 1) {				//check if CPU is available
			c.IP = head->PC;					//copy pc to IP
			endOfFile = run(c.quanta);			//call run(quanta) to run the script 
			if (endOfFile == 0) {
				if (count == 1) {
					c.IP = c.IP + 2;			//not end -> update pc pointer iwth in IP value and tail the pcb
					head->PC = c.IP;
				}
				else {
					c.IP = c.IP + 2;			//not end -> update pc pointer iwth in IP value and tail the pcb
					head->PC = c.IP;
					PCB* tmp = head;
					head = head->next;
					if (count < 3) {
						head->next = tmp;
					}
					else {
						head->next = tail;
						head->next->next = tmp;
						tail = head->next->next;
					}

				}
			}
			else {
				removeCommand(head->start, head->end);
				if (count != 1) {
					PCB* tmp = head->next;
					head->start = head->next->start;
					head->end = head->next->end;
					head->PC = head->next->PC;
					count = count - 1;
					head->next = tmp->next;
					free(tmp);
				}
				else {
					tail = NULL;
					head = NULL;
					head = tail;
					free(head);
				}
			}
		}
	}
}

int myInit(char* fileName) {
	char* loaded[3];
			f = fopen(fileName, "r");
			if (f == NULL) {
				printf("Script %s not found.\n", fileName);
				return 1;
			}
			else {
				addToRAM(f, &start, &end);
				PCB* p = makePCB(start, end);
				addToReady(p);
				fclose(f);
				return 0;
			}	
}