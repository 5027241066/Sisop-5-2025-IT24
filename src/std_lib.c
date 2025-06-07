#include "std_lib.h"

static unsigned int rand_seed;

void srand(unsigned int seed) {
    rand_seed = seed;
}

unsigned int rand() {
    rand_seed = rand_seed * 25173 + 13849;
    return rand_seed;
}

int div(int a, int b) {
    int result = 0;
    bool negative = false;
    if (a < 0) {
        negative = !negative;
        a = -a;
    }
    if (b < 0) {
        negative = !negative;
        b = -b;
    }
    if (b == 0) return -1;
    while (a >= b) {
        a -= b;
        result++;
    }
    if (negative) {
        result = -result;
    }
    return result;
}

int mod(int a, int b) {
    if (b == 0) return -1;
    if (b < 0) b = -b;
    while (a >= b) {
        a = a - b;
    }
    return a;
}

bool strcmp(char *str1, char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return false;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

void strcpy(char *dst, char *src) {
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}

void clear(byte *buf, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

void atoi(char *str, int *num) {
    int sign = 1;
    *num = 0;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str) {
        *num = (*num * 10) + (*str - '0');
        str++;
    }
    *num *= sign;
}

void itoa(int num, char *str) {
    char temp[12];
    int i = 0, j;
    bool is_negative = false;
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    while (num != 0) {
        temp[i] = mod(num, 10) + '0';
        num = div(num, 10);
        i++;
    }
    if (is_negative) {
        temp[i] = '-';
        i++;
    }
    for (j = 0; j < i; j++) {
        str[j] = temp[i - 1 - j];
    }
    str[j] = '\0';
}
