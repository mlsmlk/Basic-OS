#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"
#include "memorymanager.h"

FILE* f;
int count = 0;

PCB* head, * tail;

void boot() {
	//1. Prepare RAM for paging
	deleteRAM(0, 40);

	//2. Prepare the Backing Store
	system("rm -r BackingStore");
	system("mkdir BackingStore");
}

int kernel() {
	printf("Kernel 1.0 loaded!\n");
	return shellUI();
}

int main() {
	int error = 0;
	boot();
	error = kernel();
	return error;
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

//**TERMINATION**//

void terminate() {

	//Removing the lines from RAM
	deleteRAM(head->pageTable[0], head->pageTable[0] + (head->pages_max * 4));
	
	//Removing copied file from Backing Store
	char cmd[40];
	sprintf(cmd, "rm ./BackingStore/%d", head->pid);
	system(cmd);

	//Removing the finished pcb
	if (count != 1) {
		PCB* tmp = head->next;
		head->pid = head->next->pid;
		head->PC = head->next->PC;
		head->PC_offset = head->next->PC_offset;
		head->PC_page = head->next->PC_page;
		head->pages_max = head->next->pages_max;
		for (int i = 0; i < 10; i++) {
			head->pageTable[i] = head->next->pageTable[i];
		}
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


void scheduler() {
	//check the ready queue is completely empty -> exec function ends
	int endOfFile = 0;
	int k = 0;
	while (rQNotEmpty() == 1) {
		if (isAvailable() == 1) {				//check if CPU is available
			int frameNumber = head->pageTable[head->PC_page];
			if (frameNumber == -1) {
				/*printf("----head page table\n");
				for (int i = 0; i < 10; i++) {
					
					printf("Index: %d | Frame: %d\n", i, head->pageTable[i]);
				}*/
				pageFault(head);
			}
			c.IP = head->PC;					//copy pc to IP
			c.offset = head->PC_offset;
			
			endOfFile = run(c.quanta);			//call run(quanta) to run the script 
			if (endOfFile == 0 ||1) {
				if (endOfFile == 1) {
					pageFault(head);
				}else {
					head->PC = c.IP + c.offset;
					head->PC_offset = c.offset;
				}
				if (count != 1) {
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
			}else{
				terminate();
			}
		}
	}
}			

PCB* myInit(FILE* f) {
	int pid = fileno(f);
	PCB* p = makePCB(pid);
	addToReady(p);
	return p;
}