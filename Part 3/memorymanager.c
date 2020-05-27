#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pcb.h"
#include "ram.h"
#include "kernel.h"

//**LAUNCHER**//

FILE* file;

//4.a Counting Pages
int countTotalPages(FILE* f) {
	int counter = 0;
	char line[1000];

	while (fgets(line, 1000, f) != NULL){
		counter++;
	}
	if (counter < 4) {
		counter = 1;
	}
	else {
		counter = (counter / 4);
	}
	
	fseek(f, 0, SEEK_SET);
	return counter;
}

//4.b Loading Page to Frame
void loadPage(int pageNumber, FILE* f, int frameNumber) {
	addToRAM(pageNumber, f, frameNumber);
}

//4.c Finding Empty Frame
int findFrame() {
	return findCell();	
}

//4.d Finding Victim Frame
int findVictim(PCB* p) {
	int random = rand()%40;
	int n = random - (random % 4);
	int index = 0;
	int counter = 0;
	while (counter<10) {
		for (int i = 0; i < 10; i++) {
			if (p->pageTable[i] != n) {
				index = n;
				counter++;
			}
			else {
				n = (n + 4);
				if (n == 40) {
					n = 0;
				}
				counter = 0;
				break;
			}
		}
	}
	return n;
}

//4.e Updating Page Table
int updatePageTable(PCB* p, int pageNumber, int frameNumber, int victimFrame) {
	if (frameNumber == -1){
		p->pageTable[pageNumber] = victimFrame;
		PCB* tmp = p->next;
		while (tmp != p) {
				for (int i = 1; i < 10; i++) {
					if (tmp->pageTable[i] == victimFrame) {
						tmp->pageTable[i] = -1;
					}
				}
				tmp = tmp->next;
		}
		p->PC = victimFrame;	
	}
	else{
		p->pageTable[pageNumber] = frameNumber;
		p->PC = p->pageTable[0];
	}
}


int launcher(FILE* p) {
	PCB* pcb = myInit(p);

	//1. Copy the entire file into the backing  store
	char c; 
	char cmd[1000];
	char PID[10];
	int fd = pcb->pid;
	
	FILE* src = fdopen(fd, "r");
	sprintf(PID, "%d", fd);
	FILE* dest = fopen(PID, "w");

	while ((c = fgetc(src)) != EOF) {
		fputc(c, dest);
	}
	
	//Moving copied file to Backing Store
	strcat(cmd, "mv ");
	strcat(cmd, PID);
	strcat(cmd, " BackingStore");
	system(cmd);
	memset(cmd, 0, 1000);

	
	//2.Close the file pointer potinting to the original file
	fclose(src);
	fclose(dest);

	//3.Open the file in the backing store	
	char address[100];
	sprintf(address, "./BackingStore/%s", PID);

	file = fopen(address, "r");
	if (file == NULL) {
		return 0;
	}

	//4.Launch paging technique defaults to loading two pages of the program into RAM when it is first launched.
	int frameNumber;
	int victim =0;
	int pageNumber = countTotalPages(file);
		
	for (int i = 0; i < 2 && i < pageNumber; i++) {
		frameNumber = findFrame();
		if (frameNumber == -1) {
			victim = findVictim(pcb);
			loadPage(pageNumber, file, victim);
		}
		else {
			loadPage(pageNumber, file, frameNumber);
		}
		updatePageTable(pcb, i, frameNumber, victim);
	}
	pcb->pages_max = pageNumber;
	
	fseek(file, 0, SEEK_SET);
	memset(cmd, 0, 1000);
	memset(PID, 0, 1000);
	
	return 1;		
}


//**PAGE FAULT**//

void pageFault(PCB* p) {
	p->PC_page++;
	if (p->PC_page >= p->pages_max) {
		terminate();
	}else {
		if (p->pageTable[p->PC_page] != -1) {
			p->PC_offset = 0;
			p->PC = p->pageTable[p->PC_page];

		}else {
			//Find the page in backing store 
			char line[1000];
			char address[100];
			sprintf(address, "./BackingStore/%d", p->pid);
			FILE* f = fopen(address, "r");
			for (int i = 0; i < p->PC_page * 4; i++) {
				fgets(line, 1000, f);
			}
			//Finding space in RAM 
			int victim =0;
			for (int i = 0; i <  2 && i <(p->pages_max - (p->PC_page)); i++) {
				int frameNumber = findFrame();
				if (frameNumber == -1) {
					victim = findVictim(p);
					loadPage(p->PC_page, f, victim);
				}
				else {
					loadPage(p->PC_page, f, frameNumber);
				}
				updatePageTable(p, p->PC_page+i, frameNumber, victim);
			}
			p->PC = p->pageTable[p->PC_page];
			p->PC_offset = 0;
		}
	}
}
