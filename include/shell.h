#ifndef __SHELL_H__
#define __SHELL_H__

#include "std_type.h" // Untuk bool

void shell();
// parseCommand tetap sama, tapi implementasinya di C akan kita pastikan.
void parseCommand(char *buf, char *command, char arguments[2][64]);

#endif // __SHELL_H__
