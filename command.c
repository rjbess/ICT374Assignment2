#include <string.h>
#include "command.h"
#include <stdlib.h>
#include <stdio.h>

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
void searchRedirection(char *token[], Command *cp, int *inCounter)
{
	int i=0, result;
	char *in="<";
	char *out=">";

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
void fillCommandStructure(Command *cp,int inFirst, int inLast, char *inSuffix)
{
	cp->argc = inLast-inFirst;
	cp->commandSuffix=*inSuffix;
}
void buildCommandArgumentArray(char*token[], Command *cp, int *inCounter)
{
	int i, k=0;
	
	if(cp->stdin_file!=NULL || cp->stdout_file!=NULL)
	{
		cp->argc-=2;
	}

	if(cp->argv==NULL)
	{
		perror("Error in realloc");
		exit(1);
	}

	cp->commandPathName=token[*inCounter];
	for(i=0; i<cp->argc;(*inCounter)++)
	{	
		cp->argv[k]=token[*inCounter];
		++k;
		++i;
		
	}
	(*inCounter)++;
	cp->argv[k]=NULL;
	if(cp->stdin_file!=NULL || cp->stdout_file!=NULL)
	{
		(*inCounter)+=2;
	}
}
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
	int counter=0;
	int *counterP=&counter;
	int counterTwo=0;
	int *counterPTwo=&counterTwo;

	for(int j=0; j<c;j++)
	{
		searchRedirection(token, &command[j], counterPTwo);
		buildCommandArgumentArray(token, &command[j], counterP);
	}
	return nCommands; 
}
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
