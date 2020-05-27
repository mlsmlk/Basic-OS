#ifndef CPU_H
#define CPU_H

struct CPU {
	int IP;
	char IR[1000];
	int quanta;
};

extern struct CPU c;

int isAvailable();
int run(int quanta);

#endif