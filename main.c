#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "token.h"
#include "command.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define BUFFER MAX_NUM_TOKENS

int main()
{
	//Takes shell to ignore
	//CTRL-C, CTRL-\, CTRL-Z
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);


	char prompt[32]="%";//Reconfigurable prompt
	int numCommands=0;//Number of numbers in commandArray
	int n=1;
	int jobType;

	pid_t pid;
	int status;

	//Stores the command line
	char input[BUFFER];

	
	//Stores the command line tokens
 	//After be tokenised 
	char *tokenArray[BUFFER];
	
	//Creates 100 command structs
	Command commandArray[MAX_NUM_COMMANDS];
	
	//Clears screen
	printf("\033[2J\033[1;1H");
	
	//Loops until user exits
	//Displays prompt, takes in command line
	//Gets tokens and sets command array
	//Structure taken from Online Resource
	//"Notes on Implementation of Shell Project"
	//Original Author Hong Xie
	while(1)
	{
		//Initialises all members of array to null
		initialiseTokenArray(tokenArray);
		//Initialises all commands
		initialiseCommandArray(commandArray);
		
		//From "Notes on Implementation"
		//Original Author Hong Xie
		//Used so the slow system calles wont be
		//interrupted
		char* inputP=NULL;
		int again=1;

		printf("%s", prompt);
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
		
		//For every command in commandArray
		//Sets up redirection, piping and
		//creates a child process to run it
		for(int i=0;i<numCommands;++i)
		{
			//Exits shell
			if(strcmp(commandArray[i].argv[0],"exit")==0)
			{
				exit(0);
			}
			//Printes out the current directory
			else if(strcmp(commandArray[i].argv[0],"pwd")==0)
			{
				char cwd[128];
				printf("Current Path: %s\n", getcwd(cwd,128));
				continue;
			}
			//Changes the shell process directory
			else if(strcmp(commandArray[i].argv[0], "cd")==0)
			{
				//Will change directory to inputted value
				//if its '..' it will move back one
				if(commandArray[i].argc == 2)
				{
					chdir(commandArray[i].argv[1]);
					continue;
				}
				//Changes directory to home
				else if(commandArray[i].argc == 1)
				{
					char cwd[128];
					if(strcmp(getcwd(cwd,128), getenv("HOME"))==0)
					{
						printf("Current directory is home directory");
						continue;
					}
					else
					{
						chdir(getenv("HOME"));
						continue;
					}
				}
			}
			else if(strcmp(commandArray[i].argv[0],"cwd")==0)
			{
				chdir(commandArray[i].argv[1]);
				continue;
			}
			//Changes prompt to inputted value
			else if (strcmp(commandArray[i].argv[0],"prompt")==0)
			{
				if(strlen(commandArray[i].argv[1])>31)
				{
					printf("Invalid prompt; Too many character\n");
					continue;
				}
				strcpy(prompt, commandArray[i].argv[1]);
				continue;
			}
			//Claims zombie processes
			//From Online resource
			//'Notes on Simple Shell Implementation'
			int more=1;
			while(more)
			{
				pid =waitpid(-1, &status, WNOHANG);
				if(pid<=0)
				{
					more=0;
				}
			}
		
			//Checks command for redirection, piping
			jobType=checkJobType(&commandArray[i]);
			
			//If piping is found creates at least two
			//children processes and connects one stdout
			//to the other
			if(jobType==1)
			{
				pid_t pid2;
				int p[2];
				int np=0;
				
				//Counts how many pipes are in a row
				while(strcmp(&commandArray[i].commandSuffix, "|")==0)
				{
					i++;
					np++;
				}
				i-=np;

				//Creates pipe
				if(pipe(p)<0)
				{
					perror("Error in piping\n");
					exit(1);
				}
				//Creates base children process
				if((pid=fork())<0)
				{
					perror("Error in forking\n");
					exit(1);
				}
				if(pid==0)
				{
					dup2(p[1], STDOUT_FILENO);
					close(p[1]);
					close(p[0]);

					execvp(commandArray[i].argv[0],commandArray[i].argv);
					perror("Error in execvp");
					exit(1);
				}
				//Sets up all other children processes
				else if(pid>0)
				{
					//Creates n-1 number of child processes
					//And creates them together
					while(strcmp(&commandArray[i].commandSuffix, "|")==0)
					{
						i++;
						int fd=-1;
						//Checks if the last command has an stdout redirection
						//If so opens the file
						if(commandArray[i].stdout_file!=NULL)
						{
							fd= open(commandArray[i].stdout_file,O_WRONLY|O_CREAT, 0766);
						}
						//Creates child process
						if((pid2=fork())<0)
						{
							perror("Error in forking\n");
							exit(1);
						}
						if(pid2==0)
						{
							//If stdout found
							//closes inputs
							if(fd!=-1)
							{
								dup2(fd, STDOUT_FILENO);
								close(p[1]);
								close(p[0]);
								//execvp(commandArray[i].argv[0], commandArray[i].argv);

							}
							dup2(p[0], STDIN_FILENO);
							close(p[0]);
							close(p[1]);
							
							execvp(commandArray[i].argv[0], commandArray[i].argv);
							perror("Error in execvp\n");
							exit(1);
						}
					}
				}
				//Closes parents pipes and waits
				//for children process to finish
				if(pid>0)
				{
					close(p[0]);
					close(p[1]);
					while(np>0)
					{
						wait(&status);
						--np;
					}
				}
			}
			//Checks for stdin redirection 
			//Opens file and sets process stdin to that
			//file
			else if(jobType==2)
			{
			
				int fd;
				fd=open(commandArray[i].stdin_file, O_RDONLY|O_CREAT, 0766);

				if((pid=fork())<0)
				{
					perror("Error in forking");
					exit(1);
				}
				if(pid==0)
				{
					dup2(fd, STDIN_FILENO);
					execvp(commandArray[i].argv[0], commandArray[i].argv);
					perror("Error in execvp");
					exit(1);
				}

			}
			//Checks for stdout redirection
			//If found opens file and directs
			//process stdout to file
			else if(jobType==3)
			{
				int fd;
				fd= open(commandArray[i].stdout_file,O_WRONLY|O_CREAT, 0766);
				if((pid=fork())<0)
				{
					perror("Error in forking");
					exit(1);
				}
				if(pid==0)
				{
					dup2(fd, STDOUT_FILENO);
					execvp(commandArray[i].argv[0], commandArray[i].argv);
					perror("Error in execvp");
					exit(1);
				}

			}
			//Standard process, no piping,
			//no redirection
			else
			{
				if((pid=fork())<0)
				{
					perror("Error in forking\n");
				        exit(1);
				}
				if(pid==0)
				{
					execvp((commandArray[i].argv[0]),((commandArray[i].argv)));
				        perror("Error in execvp\n");
				        exit(1);
				}
			}
			//If its a background process continues
			//to next command
			if(strcmp(&(commandArray[i].commandSuffix),"&")==0)
			{
				continue;
			}
			//Sequential process, shell will wait 
			//until child has finished
			else
			{
				wait(&status);
			}
		}
	}
	return 0;
}
