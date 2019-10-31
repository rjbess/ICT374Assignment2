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

	char input[BUFFER];
	char *tokenArray[BUFFER];
	initialiseTokenArray(tokenArray);
	
	Command commandArray[MAX_NUM_COMMANDS];
	initialiseCommandArray(commandArray);
	
	while(1)
	{
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
		tokenise(input, tokenArray);
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
		//kept in main for debugging
		for(int i =0; i<numCommands;i++)
		{
			printComStruct(&(commandArray[i]));
		}
	}
	return 0;
}
