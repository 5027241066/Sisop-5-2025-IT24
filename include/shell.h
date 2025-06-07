#ifndef __SHELL_H__
#define __SHELL_H__

#include "std_type.h"

void shell();
void parseCommand(char *buf, char *cmd, char arg[2][64]);
void handleCommand(char *cmd, char arg[2][64], char *buf);
void updatePrompt();
void grandCompany(char* company);

#endif // __SHELL_H__
