#include "token.h"

int tokenise(char *inputLine, char *inToken[])
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
	return count;
}
