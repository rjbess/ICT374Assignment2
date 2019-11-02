#ifndef COMMAND_C
#define COMMAND_C

#include <string.h>
#include "command.h"
#include <stdlib.h>
#include <stdio.h>

//Returns 1 if the token is a command separator
//0 if not. Taken from Lab 8 command.c
int separator(char *token)
{
	int i=0;
	char *commandSeparators[] = {pipeSep, conSep, seqSep, NULL};

	while (commandSeparators[i] != NULL) 
	{
		if (strcmp(commandSeparators[i], token) == 0)
		{
			return 1; 
		} 
		++i;
	}
	return 0;
}
//Searches the inputted token array
//for stdin/ stdout redirection ('<'|'>')
//If found sets the corresponding commands redirction to that
//token. inCounter is used to count the token array
//for each command 
void searchRedirection(char *token[], Command *cp, int *inCounter)
{
	int i=0, result;
	char *in="<";
	char *out=">";
	
	//Seaches the inputted token array for a redirection
	//Will only loop the token array until one is found or 
	//the number of argumemts
	while(i<cp->argc && cp->stdin_file==NULL && cp->stdout_file==NULL)
	{	
		result=strcmp(token[*inCounter], in);
		if(result==0)
		{
			cp->stdin_file=token[(*inCounter)+1];
			(*inCounter)++;
		}
		result=strcmp(token[*inCounter], out);
		if(result==0)
		{
			cp->stdout_file=token[(*inCounter)+1];
			(*inCounter)++;
		}
		(*inCounter)++;
		i++;
	}
}
//Sets the commands argc (Number of arguments, including redirction)
//And its command Suffix
void fillCommandStructure(Command *cp,int inFirst, int inLast, char *inSuffix)
{
	cp->argc = inLast-inFirst;//Number of command arguments
	cp->commandSuffix=*inSuffix;//Command suffix- ; & |
}
//For the inputted commmand will build its argv (command argument array)
void buildCommandArgumentArray(char*token[], Command *cp, int *inCounter)
{
	int i, k=0;
	
	//If redirection is found number of arguments is reduced by two
	if(cp->stdin_file!=NULL || cp->stdout_file!=NULL)
	{
		cp->argc-=2;
	}

	if(cp->argv==NULL)
	{
		perror("Error no commands found, exiting");
		exit(1);
	}

	//Sets command path name to first command
	cp->commandPathName=token[*inCounter];
	
	//Loops around for number of command arguments and sets command
	//arguement array
	for(i=0; i<cp->argc;(*inCounter)++)
	{	
		cp->argv[k]=token[*inCounter];
		++k;
		++i;
		
	}
	(*inCounter)++;
	cp->argv[k]=NULL;

	//If rediection is found will increment the counter by two
	//so the next command array is correct
	if(cp->stdin_file!=NULL || cp->stdout_file!=NULL)
	{
		(*inCounter)+=2;
	}
}
//Built upon Lab 8 and 9
//Original Author Hong Xie
//The main function which sets each command
//and its elements
int separateCommands(char *token[], Command command[])
{
	int i;
	int nTokens;

	// find out the number of tokens
	i = 0;
	while (token[i] != NULL) ++i; 
		nTokens = i;
	// if empty command line
	if (nTokens == 0) 
		return 0;
	// check the first token
	if (separator(token[0]))
		return -3;
	// check last token, add ";" if necessary 
	if (!separator(token[nTokens-1])) {
		token[nTokens] = seqSep;
	        ++nTokens;
	}
	int first=0; // points to the first token of a command
	int last;      // points to the last  tokens of a command
	char *sep=NULL;     // command separator at the end of a command
	int c = 0;// command index
	
	for (i=0; i<nTokens; ++i)
	{
		last=i;
		if (separator(token[i])) 
		{
			sep = token[i];
		 	if (first==last)  // two consecutive separators
				return -2;
		 	fillCommandStructure(&(command[c]),first,  last, sep);
		 	++c;
		 	first = i+1; 
		}
	}
	// check the last token of the last command  
	if (strcmp(token[last], pipeSep) == 0)
	{ // last token is pipe separator
		return -4; 
	} 
	// calculate the number of commands
	int nCommands = c;
	//Used to count each token in tokenArray
	//and for every command set its redirection
	//As such can't be reset after each command
	int counter=0;
	int *counterP=&counter;
	//USed to count each token in tokenArray
	//and for every command build its argument array
	//As such can't be reset after each command
	int counterTwo=0;
	int *counterPTwo=&counterTwo;

	//For each command set redirection and build its argument array
	for(int j=0; j<c;j++)
	{
		searchRedirection(token, &command[j], counterPTwo);
		buildCommandArgumentArray(token, &command[j], counterP);
	}
	return nCommands;//Number of commands
}
//Used mainly for debugging
//Prints out all attributes of a single command
//USed from online resource
//"Notes of Implementation of Shll Project"
void printComStruct(Command *com)
{ 
	int i;

	fprintf(stderr,"com_pathname=%s\n", com->commandPathName); 
	fprintf(stderr,"argc=%d\n", com->argc); 
	for(i=0; i<com->argc; i++)
	{
		fprintf(stderr,"argv[%d]=%s\n", i, com->argv[i]);
	}
	fprintf(stderr,"#######\n"); 
	if (com->stdin_file == NULL)
	{
		fprintf(stderr,"redirect_in=NULL\n"); 
	}
	else
	{
		fprintf(stderr,"redirect_in=%s\n", com->stdin_file); 
	}
	if (com->stdout_file == NULL)
	{
		fprintf(stderr,"redirect_out=NULL\n"); 
	}
	else
	{
		fprintf(stderr,"redirect_out=%s\n", com->stdout_file); 
	}
	fprintf(stderr,"com_suffix=%c\n\n", com->commandSuffix);
}
//Initialise all commands in the commandArray
void initialiseCommandArray(Command command[])
{
	for(int i=0;i<MAX_NUM_COMMANDS;i++)
	{
		command[i].commandPathName=NULL;
		command[i].argc=0;
		for(int j=0;j<MAX_NUM_ARGUMENTS;j++)
		{
			command[i].argv[j]=NULL;
		}
		command[i].commandSuffix=' ';
		command[i].stdin_file=NULL;
		command[i].stdout_file=NULL;
	}
}
#endif
