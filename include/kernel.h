#include "std_type.h"

#ifndef __KERNEL_H__
#define __KERNEL_H__

 // Untuk byte

// Fungsi-fungsi dari kernel.asm
extern void putInMemory(int segment, int address, char character);
// Pastikan signature ini sesuai dengan bagaimana kernel.asm (32-bit) mengambil parameter.
// Jika kernel.asm menggunakan EAX, EBX, dll., maka di C tetap int, tapi pemanggilannya dari C
// akan sesuai dengan konvensi C yang menempatkan argumen di stack.
extern int interrupt(int number, int AX, int BX, int CX, int DX); 
unsigned int getBiosTick();

// Fungsi-fungsi dari kernel.c
void printString(char* str);
void readString(char* buf);
void clearScreen(); // Fungsi ini di kernel.c akan menggunakan atribut global

// TAMBAHAN: Fungsi untuk mengatur atribut warna global yang digunakan oleh clearScreen dan printString
void setKernelPrintAttribute(byte attribute);

#endif // __KERNEL_H__
