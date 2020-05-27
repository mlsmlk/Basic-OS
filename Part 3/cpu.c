#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include "ram.h"

typedef struct CPU {
	int IP;
	char IR[1000];
	int quanta ;
	int offset;
}CPU;

struct CPU c = {0,0,2,0};

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

	for (int i = c.IP; i < c.IP + c.quanta + c.offset ; i++) {
		if (c.offset < 4 && c.quanta >0) {
			for (int j = 0; j < 1000; j++)
			{
				if (ram[i] == NULL) {
					//End of file, run terminate
					return 2;
				}
				else {
					c.IR[j] = ram[i][j];
				}
			}
			parse(c.IR);
			c.offset++;
			c.quanta--;
		}else if( c.offset = 4){
			//End of page, run page fault
			return 1;
		}
	}
	return 0;
}