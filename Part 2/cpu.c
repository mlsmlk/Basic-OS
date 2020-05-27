#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include "ram.h"

typedef struct CPU {
	int IP;
	char IR[1000];
	int quanta ;
}CPU;

struct CPU c = {0,0,2};

int isAvailable() {
	if (c.quanta < 2) {
		c.quanta = 2;
		return 0;
	}else {
		return 1;
	}
}

int run(int quanta) {
	char command[1000];
	for (int i = c.IP; i < c.IP + 2; i++) {
		if(strcmp(ram[i], "end") !=0){
		for (int j = 0; j < 1000; j++)
		{
			c.IR[j] = ram[i][j];
		}
		parse(c.IR);
		c.quanta --;

		}else return 1;
	}
	return 0;
}