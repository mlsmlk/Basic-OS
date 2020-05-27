#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct MEM {
	char* var;
	char* string;
} memory;

static memory mem[1000] = { 0 };
int mem_size = 0;

int set(char *words[]){
	bool exists = false;	
	char *var = words[1];
	char* string = words[2];
	for (int i = 0; i < mem_size; i++){
		if (strcmp(mem[i].var, var) == 0){	//check if the variable exists
			mem[i].string = string;
			exists = true;
			break;
		}
	}
	if (exists == false){	//create new var in memory bc variable does not exist
		mem[mem_size].var = var;
		mem[mem_size].string = string;
		mem_size++;
	}
	return 0;
}

int print(char* var){
	bool exists = false;
	for (int i = 0; i < mem_size; i++){	//check if the variable exist
		if (strcmp(mem[i].var, var) == 0){
			printf("%s\n", mem[i].string);
			exists = true;
			break;
		}
	}
	if (exists == false){ //throws error if the variable does not exist
		return 3;
	}
	return 0;
}