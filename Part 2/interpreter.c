#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "kernel.h"

int help() {
	printf(	"COMMAND LIST			DESCRIPTION\n"
			"help				Displays all the commands\n"
			"quit				Exits\n"
			"set VAR STRING			Assigns a value to shell memory\n"
			"print VAR			Displays the STRING assigned to VAR\n"
			"run SCRIPT.TXT			Executes the file SCRIPT.TXT\n"
			"exec p1 p2 p3			Executes concurrent programs\n");
	return 0;
}
int in_file_flag = 0;

int quit() {
	printf("Bye!\n");
	if (in_file_flag == 0) {
		return -1;
	}
	else {
		return 0;
	}
}

int runScript(char * f){
	int errCode = 0;
	char line[1000];

	FILE *file = fopen(f, "r");
	if (file == NULL) return 2;
	in_file_flag = 1;
	while (fgets(line, 999, file)){
		errCode = parse(line);
		if (feof(file)) {
			printf("End of file reached.\n");
		}	
		if (errCode != 0){
			fclose(file);
			return errCode;
		}
	}
	in_file_flag = 0;
	fclose(file);
	return errCode;
}
int exec(char* words[], int w){
	char* loaded[3];
	int duplicationFlag = 0;
	int counter = 0;
	int nullFlag = 0;
	for (int i = 1; i < w; i++) {
		for (int j = 0; j < counter; j++) {
			if (strcmp(words[i], loaded[j]) == 0) {
				duplicationFlag = 1;
				printf("Error: Script %s already loaded.\n", words[i]);
				return 0;
			}
		}

		if(duplicationFlag == 0){
			nullFlag = myInit(words[i]);
			if (nullFlag == 1) {
				return 0;
			}else{
				loaded[i - 1] = words[i];
				counter++;
			}
		}
	}
	scheduler();
	for (int i = 0; i < counter; i++) {
		loaded[i] = NULL;
	}
	return 0;
}

int set(char* words[]);
int print(char* var);
int interpreter(char* words[], int w);

int parse(char ui[]) {
	char tmp[200]; 
	int a, b;
	char* words[100]; 
	int w = 0; // word index
	b = 0;
	for (a = 0; a <= (strlen(ui)); a++) {
		if (ui[a] == ' ' || ui[a] == '\0' || ui[a] == '\n' || ui[a] == '\r') {
			if (b != 0) {
				tmp[b] = '\0';
				words[w] = strdup(tmp);
				w++;
				b = 0;
			}
		}else {
			tmp[b] = ui[a];
			b++;
		}
	}
	return interpreter(words, w);
}

int interpreter(char* words[], int w) { 
		int errCode = 0;
		if (strcmp(words[0], "help")==0) errCode = help();
		else if (strcmp(words[0], "quit")==0) errCode = quit();
		else if (strcmp(words[0], "set") == 0) {
			if (w > 2) errCode = set(words);
			else {
				printf("This command requires at least two arguments. For more informartion, type help.\n");
			}
		}
		else if (strcmp(words[0], "print") == 0) {
			if (w == 2) errCode = print(words[1]);
			else {
				printf("This command requires one argument. For more informartion, type help.\n");
			}
		}
		else if (strcmp(words[0], "run")==0) {
			if (w == 2) errCode = runScript(words[1]);
			else {
				printf("This command requires one argument. For more informartion, type help.\n");
			} 
		} 
		else if (strcmp(words[0], "exec") == 0) {
			if (w > 1 && w < 5) errCode = exec(words, w);
			else {
				printf("This command accepts one to three arguments. For more information, type help.\n");
			}
		}
		else errCode = 1;
			return errCode;
}

