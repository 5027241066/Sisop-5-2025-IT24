#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "std_type.h"

// Extern functions from kernel.asm
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int number, int AX, int BX, int CX, int DX);
extern unsigned int getBiosTick();

// Functions from kernel.c
void printString(char* str);
void readString(char* buf);
void clearScreen();
void setTextColor(byte color); 

#endif // __KERNEL_H__
