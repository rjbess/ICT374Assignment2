#include <string.h>

void exitCommand(Command inCommandArray[], int inInt)
{
	for (int i=0;i<inInt; i++)
	{
		if(strcmp(inCommandArray[i].argv[0], "exit")==0)
		{
			exit(0);
		}
	}
}
