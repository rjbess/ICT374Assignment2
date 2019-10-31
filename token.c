#include "token.h"
#include <string.h>

void tokenise(char *inputLine, char *inToken[])
{
	char *token;
	int count=0;

	token= strtok(inputLine, seperatingChars);
	inToken[count]=token;

	while(token!=NULL&& count<MAX_NUM_TOKENS)
	{
		++count;
		token=strtok(NULL, seperatingChars);
	
		inToken[count]=token;
		
	}
}
void initialiseTokenArray(char* inToken[])
{
	for(int i=0;i<MAX_NUM_TOKENS;i++)
	{
		inToken[i]=NULL;
	}
}
