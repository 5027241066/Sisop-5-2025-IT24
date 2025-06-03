#ifndef __STD_LIB_H__
#define __STD_LIB_H__

#include "std_type.h" // Untuk tipe byte dan bool

// Deklarasi fungsi-fungsi dari std_lib.c
int div(int a, int b);
int mod(int a, int b);

bool strcmp(char *str1, char *str2);
void strcpy(char *dst, char *src);
void clear(byte *buf, unsigned int size);

void atoi(char *str, int *num); // Sesuai signature yang kamu minta
void itoa(int num, char *str);

#endif // __STD_LIB_H__
