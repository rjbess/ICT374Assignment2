#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "token.h"
#include "command.h"
#include "menu.h"

#include <unistd.h>
#include <sys/types.h>

#define BUFFER MAX_NUM_TOKENS
#define PIPEBUFFER 512

int main()
{
	pid_t pid;
	int p[2];

	char *prompt="%";
	int numCommands=0;

	//Stores the command line
	char input[BUFFER];

	
	//Stores the command line tokens
 	//After be tokenised 
	//Initialised to null
	char *tokenArray[BUFFER];
	initialiseTokenArray(tokenArray);
	
	//Creates 100 command structs
	//And initialises its values
	Command commandArray[MAX_NUM_COMMANDS];
	initialiseCommandArray(commandArray);
	
	//Loops until user exits
	//Displays prompt, takes in command line
	//Gets tokens and sets command array
	//Structure taken from Online Resource
	//"Notes on Implementation of Shell Project"
	//Original Author Hong Xie
	while(1)
	{
		//From "Notes on Implementation"
		//Original Author Hong Xie
		//Used so the slow system calles wont be
		//interrupted
		char* inputP=NULL;
		int again=1;

		displayPrompt(prompt);
		while(again)
		{
			again=0;
			inputP=fgets(input, BUFFER, stdin);
			if(inputP==NULL)
			{
				if(errno==EINTR)
				{
					again=1;
				}
			}
		}
		//Tokenises command line
		tokenise(input, tokenArray);
		//Converts tokens into the command struct
		numCommands=separateCommands(tokenArray, commandArray);
		
		for(int i=0;i<numCommands;i++)
		{
			if(strcmp(commandArray[i].argv[0], "exit")==0)
			{
				exit(0);
			}
		
			//create child processes
				//conditional jump or move depends
				//on unitilised variable
			/*if(strcmp(&(commandArray[i].commandSuffix), "|")==0)
			{
					
			}*/
				
		//background job
			//continue
		//wait job finish
		}
		//Loops through all commands
		//and prints them out
		//kept in main for debugging
		for(int i =0; i<numCommands;i++)
		{
			printComStruct(&(commandArray[i]));
		}
	}
	return 0;
}
