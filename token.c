#include "token.h"
#include <string.h>

//Takes in the command line and splits it up into tokens
//on | & ; and stores in tokenArray
//Build in Lab7
void tokenise(char *inputLine, char *inToken[])
{
	char *token;
	int count=0;

	//Separates on | & ;
	token= strtok(inputLine, seperatingChars);
	inToken[count]=token;

	while(token!=NULL&& count<MAX_NUM_TOKENS)
	{
		++count;
		token=strtok(NULL, seperatingChars);
	
		inToken[count]=token;
		
	}
}
//Initialises all values in tokenArray to NULL
void initialiseTokenArray(char* inToken[])
{
	for(int i=0;i<MAX_NUM_TOKENS;i++)
	{
		inToken[i]=NULL;
	}
}
