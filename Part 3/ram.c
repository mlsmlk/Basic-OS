#include <stdio.h>
#include <string.h>
#include "ram.h"

// find the next available cell in ram[]
int findCell() {
	int position = -1;
	for (int i = 0; i < 40; i++) {
		if (ram[i] == NULL) {
			position = i;
			break;
		}
	}
	return position;
}

//loading RAM
void addToRAM(int pageNumber,FILE *f,  int frameNumber) {
	int index = 0;
	char commands[1000];
	while (fgets(commands, 999, f)) {
		ram[frameNumber] = strdup(commands);
		frameNumber++;
		index++;
		if (index == 4) {
			break;
		}
	}
}

//removing from ram
void deleteRAM(int start, int end) {
	for ( int i = start; i < end ; i++) {
		ram[i] = NULL;
	}
	
}