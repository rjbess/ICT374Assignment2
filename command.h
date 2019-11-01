#define MAX_NUM_COMMANDS  100
#define MAX_NUM_ARGUMENTS 1000
#define pipeSep  "|"
#define conSep   "&"
#define seqSep   ";"

//Holds a single command
//Built on Lab 8 and 9 work
struct CommandStruct
{
	char* commandPathName;//Name of command, argv[0]
	int argc;// Number of arguments including redirections
	char *argv[MAX_NUM_ARGUMENTS];// Array of command arguments not including redirection
   		
	char commandSuffix;//Command suffix '|', ';', '&'
	char *stdin_file;//NULL or stdin value
	char *stdout_file;//NULL or stdout value
};

typedef struct CommandStruct Command;  // command type

void buildCommandArgumentArray(char *token[], Command *cp, int *inCounter);
void printComStruct(Command *command);
int separateCommands(char *token[], Command command[]);
void initialiseCommandArray(Command command[]);
