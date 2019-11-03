#ifndef EXECUTECOMMANDS_C
#define EXECUTECOMMANDS_C

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "executeCommands.h"
#include <string.h>

int executePipeCommand(char** pipe1, char** pipe2)
{
	int p[2], status;
	int n=2;
	pid_t pid, pid2;

	if(pipe(p)<0)
	{
		perror("Error in piping\n");
		exit(1);
	}
	if((pid=fork())<0)
	{
		perror("Error in forking\n");
		exit(1);
	}
	if(pid==0)
	{
		dup2(p[1], STDOUT_FILENO);
		close(p[0]);
		close(p[1]);
		
		if((execvp(pipe1[0], pipe1))<0)
		{
			perror("Error in execvp\n");
			exit(EXIT_FAILURE);
		}
	}
	else if(pid>0)
	{
		if((pid2=fork())<0)
		{
			perror("Error in forking\n");
			exit(1);
		}
		if(pid2==0)
		{
			dup2(p[0], STDIN_FILENO);
			close(p[1]);
			close(p[0]);
		
			if((execvp(pipe2[0], pipe2))<0)
			{
				perror("Error in execvp\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	if(pid>0)
	{
		close(p[0]);
		close(p[1]);
		while(n>0)
		{
			wait(&status);
			--n;
		}
		return 0;
	}
}
int checkJobType(Command *inCommand)
{
	if((inCommand->stdin_file) ==NULL && (inCommand->stdout_file)==NULL && ((strcmp(&(inCommand->commandSuffix), ";")==0)||(strcmp(&(inCommand->commandSuffix), "&")==0)))
	{
		return 0;
	}
	/*else if(strcmp(&(inCommand->commandSuffix), "|")==0)
	{
		return 1;
	}*/
	else if(inCommand->stdin_file!=NULL)
	{
		return 2;
	}
	else if(inCommand->stdout_file!=NULL)
	{
		return 3;
	}
	else 
	{
		return 1;
	}
}
#endif
