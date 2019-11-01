#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "executeCommands.h"

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
		close(p[0]);
		dup2(p[1], STDOUT_FILENO);
		close(p[1]);

		execvp(pipe1[0], pipe1);

		perror("Error in execvp\n");
		exit(1);
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
			close(p[1]);
			dup2(p[0], STDIN_FILENO);
			close(p[0]);

			execvp(pipe2[0], pipe2);

			perror("Error in execvp\n");
			exit(1);
		}
		if(pid>0)
		{
			while(n>0)
			{
				pid=wait(&status);
				--n;
			}
			return 0;
		}
	}
}
