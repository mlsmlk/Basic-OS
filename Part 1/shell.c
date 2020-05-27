#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

void greeting_shell(){
	printf("Welcome to the Melis Malki's shell!\n");
	printf("Version 1.0 Created January 2020\n");
}
int parse(char ui[]) {
	char tmp[200]; int a, b;
	char* words[100]; int w = 0; // word index
	ui[strlen(ui) - 1] = '\0';	//replaces the \n at the end with a /0
	
	for (a = 0; ui[a] == ' ' && a < 1000; a++); // skip white spaces
	while (ui[a] != '\0' && a < 1000) {
		for (b = 0; ui[a] != '\0' && ui[a] != ' ' && a < 1000; a++, b++) {
			tmp[b] = ui[a]; // extract a word
		}
		tmp[b] = '\0';
		words[w] = strdup(tmp);
		a++; w++;
	}
	return interpreter(words,w); 
}
int main() {
	greeting_shell();
	char prompt[100] = { '$','\0' };
	char userInput[1000]; 
	int errorCode = 0; //initiliaze error to no error

	while (1) { 
		printf("%s", prompt);
		fgets(userInput, 999, stdin); // limit input to array size
		errorCode = parse(userInput);
		if (errorCode == -1) exit(99);// ignore all other errors and exit
		else if (errorCode == 1) printf("Unknown command.\n");
		else if (errorCode == 2) printf("Script not found.\n");
		else if (errorCode == 3) printf("Variable does not exist.\n");
		else if (errorCode == 4) printf("This command requires at least two arguments. For more informartion, type help.\n");
		else if (errorCode == 5) printf("This command requires one argument. For more informartion, type help.\n");
	}
}
