#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"


int shellUI() {
	printf("Welcome to the Melis Malki's shell!\n"
			"Shell Version 2.0 Updated February 2020\n");

	char prompt[100] = { '$','\0' };
	char userInput[1000]; 
	
	// Error initialization (0 =No Error)
	int errorCode = 0; 

	while (1) { 
		printf("%s", prompt);

		memset(userInput, 0, 1000);
		fgets(userInput, 999, stdin);

		errorCode = parse(userInput);
		
		if (errorCode == -1) exit(99);// ignore all other errors and exit
		else if (errorCode == 1) printf("Unknown command.\n");
		else if (errorCode == 2) printf("Script not found.\n");
		else if (errorCode == 3) printf("Variable does not exist.\n");
		}
}