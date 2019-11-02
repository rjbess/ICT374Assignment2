#ifndef EXECUTECOMMANDS_H
#define EXECUTECOMMAND_H

#include "command.h"

int executePipeCommand(char** pipe1, char** pipe2);
int checkJobType(Command* command);

#endif
