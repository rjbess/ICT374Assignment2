#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

void clearMenu()
{
	printf("\033[2J\033[1;1H");
}
void displayMenu()
{
	clearMenu();
	printf("\n\n\n\n");
	printf("*******************************************");
	printf("\n\n");
	printf("    ICT374 Assignment Two- Simple Shell\n");
	printf("          Robert Bessell 33579862\n");
	printf("\n\n");
	printf("*******************************************");
	printf("\n\n\n\n");
}
void displayPrompt(char* inPrompt)
{	
	printf("%s", inPrompt);
}

