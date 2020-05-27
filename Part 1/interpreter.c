
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "shell.h"

void help() {
	printf("COMMAND LIST\t DESCRIPTION\n");
	printf("help \t\t Displays all the commands\n");
	printf("quit\t\t Exits\n");
	printf("set VAR STRING\t Assigns a value to shell memory\n");
	printf("print VAR\t Displays the STRING assigned to VAR\n");
	printf("run SCRIPT.TXT\t Executes the file SCRIPT.TXT\n");
	return 0;
}

int quit() {
	printf("Bye!\n");
	return -1;
}

int run(char * f){
	int errCode = 0;
	char line[1000];

	FILE *file = fopen(f, "r");
	if (file == NULL) return 2;

	while (fgets(line, 999, file)){
		errCode = parse(line);
		if (errCode != 0){
			fclose(file);
			return errCode;
		}
	}

	fclose(file);
	return errCode;
}

int interpreter(char* words[], int w) { 
// word[0] = command
	int errCode = 0;
		if (strcmp(words[0], "help")==0) help();
		else if (strcmp(words[0], "quit")==0) errCode = quit();
		else if (strcmp(words[0], "set") == 0) {
			if (w > 2) errCode = set(words);
			else errCode = 4;
		}
		else if (strcmp(words[0], "print") == 0) {
			if (w == 2) errCode = print(words[1]);
			else errCode = 5;
		}
		else if (strcmp(words[0], "run")==0) {
			if (w == 2) errCode = run(words[1]);
			else errCode = 5;
		} 
		else errCode = 1;
			return errCode;
}