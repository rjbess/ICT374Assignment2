#define MAX_NUM_COMMANDS  100
#define MAX_NUM_ARGUMENTS 1000
#define pipeSep  "|"
#define conSep   "&"
#define seqSep   ";"

struct CommandStruct
{
	char* commandPathName;
	int argc;
	char *argv[MAX_NUM_ARGUMENTS];
   		
	char commandSuffix;
	char *stdin_file;
	char *stdout_file;
};

typedef struct CommandStruct Command;  // command type

void buildCommandArgumentArray(char *token[], Command *cp, int *inCounter);
void printComStruct(Command *command);
int separateCommands(char *token[], Command command[]);
void initialiseCommandArray(Command command[]);
