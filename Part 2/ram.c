#include <stdio.h>
#include <string.h>
#include "ram.h"

// find the next available cell in ram[]
int findCell() {
	int position = 0;
	for (int i = 0; i <1000; i++) {
		if (ram[i] == NULL) {
			position = i;
			break;
		}
	}
	return position;
}
//loading RAM

void addToRAM(FILE * f, int* start, int* end) {
	int arr[333];
	int k;
	int i = 0;
	char buffer[1000];
	while (fgets(buffer, 999, f)) {
		k =findCell();
		arr[i] = k; //record the positions
		ram[k] = strdup(buffer);
		i= i+ 1;
	}
	*start = arr[0];
	*end = arr[i-1];
	ram[k + 1] = "end";
}


//removing from ram
void removeCommand(int start, int end) {
	for ( int i = start; i < end+1 ; i++) {
		ram[i] = NULL;
	}
	
}