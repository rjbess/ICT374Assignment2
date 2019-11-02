#ifndef TOKEN_H
#define TOKEN_H

#define MAX_NUM_TOKENS 100000
#define seperatingChars "\n\t "

void tokenise(char *inputLine, char *token[]);
void initialiseTokenArray(char *[]);

#endif
