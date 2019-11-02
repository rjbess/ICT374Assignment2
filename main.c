#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "token.h"
#include "command.h"
#include "menu.h"
#include "executeCommands.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER MAX_NUM_TOKENS

int main()
{
	char *prompt="%";
	int numCommands=0;
	int jobType;
	
	pid_t pid;
	int status;

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
		
		for(int i=0;i<numCommands;++i)
		{
			if(strcmp(commandArray[i].argv[0],"exit")==0)
			{
			exit(0);
			}
	
			if(strcmp(commandArray[i].argv[0]),"pwd")==0)
			{
			printf("Current Path: %s", getcwd(path,100));
			}
		
			if(strcmp(commandArray[i].argv[0], "cd")==0)
			{
				if(commandArray[i].argc = 2)
				{
					chdir(commandArray[i].argv[1]);
				}

				else if(commandArray[i].argc = 1)
				{
					if(strcmp(getcwd(path,100), getenv("HOME"))==0)
					{
						printf("Current directory is home directory");
					}
					else
					{
						chdir(getenv("HOME"));
					}
				}
			}

			if(strcmp(commandArray[i].argv[0]),"cwd")==0)
			{
				chdir(commandArray[i].argv[1]);
			}


			if (strcmp(commandArray[i].argv[0],"prompt")==0)
			{
				strcpy(prompt, commandArray[i].argv[1]);
			}			
			
			int more=1;
			while(more)
			{
				pid =waitpid(-1, &status, WNOHANG);
				if(pid<=0)
				{
					more=0;
				}
			}
			jobType=checkJobType(&commandArray[i]);

			if(jobType==0 && pid==0)
			{
				if((pid=fork())<0)
				{
					perror("Error in forking\n");
					exit(1);
				}
				execvp((commandArray[i].argv[0]),((commandArray[i].argv)));
				perror("Error in execvp\n");
				exit(1);
			
			}
			if(jobType==1)
			{
				int p[2];
				int n=2;
				pid_t pid2;

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

					execvp(commandArray[i].argv[0],commandArray[i].argv);
					perror("Error in execvp");
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
						dup2(p[0], STDIN_FILENO);
						close(p[1]);
						close(p[0]);

						execvp(commandArray[i+1].argv[0], commandArray[i+1].argv);
						perror("Error in execvp\n");
						exit(1);
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
				}
				i++;
			}
			if(strcmp(&(commandArray[i].commandSuffix),"&")==0)
			{
				continue;
			}
			else
			{
				wait(&status);	
			}
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
