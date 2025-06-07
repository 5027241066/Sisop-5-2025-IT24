#include "shell.h"
#include "kernel.h"

byte video_attribute = 0x07;

void main() {
    clearScreen();
    shell();
}

void printString(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        int ax = 0x0E00 | str[i];
        int bx = 0x0000 | video_attribute;
        interrupt(0x10, ax, bx, 0, 0);
        i++;
    }
}

void readString(char *buf) {
    int i = 0;
    char c;

    while (true) {
        int ax = interrupt(0x16, 0x0000, 0, 0, 0);
        c = (char) (ax & 0xFF);

        if (c == '\r') {
            buf[i] = '\0';
            printString("\r\n");
            return;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                interrupt(0x10, 0x0E00 | '\b', 0, 0, 0);
                interrupt(0x10, 0x0E00 | ' ', 0, 0, 0);
                interrupt(0x10, 0x0E00 | '\b', 0, 0, 0);
            }
        } else {
            buf[i] = c;
            interrupt(0x10, 0x0E00 | c, 0, 0, 0);
            i++;
        }
    }
}

void clearScreen() {
    int i;
    for (i = 0; i < 80 * 25; i++) {
        putInMemory(0xB800, i * 2, ' ');
        putInMemory(0xB800, i * 2 + 1, video_attribute);
    }
    interrupt(0x10, 0x0200, 0, 0, 0);
}

void setTextColor(byte color) {
    video_attribute = color;
}
