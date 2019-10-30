#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.c"
#include "command.c"
#include <unistd.h>
#include <sys/types.h>

#define BUFFER 128

int main()
{
	pid_t pid;
	char input[BUFFER];
	char *tokenArray[MAX_NUM_TOKENS];
	for(int i=0;i<MAX_NUM_TOKENS;i++)
	{
		tokenArray[i]=NULL;
	}
	int numTokens=0,numCommands=0;

	printf("$");
	fgets(input, BUFFER, stdin);
	
	numCommands=tokenise(input, tokenArray);
	Command commandArray[numCommands];

	for(int i=0;i<numCommands;i++)
	{
		commandArray[i].commandPathName=NULL;
		commandArray[i].argc=0;

		for(int j=0;j<MAX_NUM_ARGUMENTS;j++)
		{
			
			commandArray[i].argv[j]=NULL;
		}
		commandArray[i].commandSuffix=' ';
		commandArray[i].stdin_file=NULL;
		commandArray[i].stdout_file=NULL;
	}
	numTokens=separateCommands(tokenArray, commandArray);
	for(int i=0;i<numTokens;i++)
	{
		printComStruct(&(commandArray[i]));
	}
	pid=fork();
	if(pid==0)
	{
		execvp(commandArray[0].argv[0], commandArray[0].argv);
	}
	return 0;
}
