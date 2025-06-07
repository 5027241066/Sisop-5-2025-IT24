#include "shell.h"
#include "kernel.h"
#include "std_lib.h"

// ... (variabel global dan deklarasi fungsi helper tetap sama) ...
char username[64];
char prompt_suffix[16];
char prompt[128];
byte current_color = 0x07;
int strlen(char* str);


void shell() {
    char buf[128];
    char cmd[16];
    char arg[2][64];

    // >>> PERUBAHAN 1: Seed generator acak HANYA SEKALI saat shell dimulai <<<
    srand(getBiosTick());

    // Inisialisasi awal
    strcpy(username, "user");
    strcpy(prompt_suffix, "> ");
    updatePrompt();

    printString("Welcome to EorzeOS!\r\n");

    while (true) {
        printString(prompt);
        readString(buf);
        parseCommand(buf, cmd, arg);
        handleCommand(cmd, arg, buf);
    }
}

// ... (updatePrompt, parseCommand, grandCompany tetap sama) ...
// ... (Salin fungsi-fungsi ini dari jawaban sebelumnya) ...
void updatePrompt() {
    clear(prompt, 128);
    strcpy(prompt, username);
    strcpy(prompt + strlen(username), prompt_suffix);
}
void parseCommand(char *buf, char *cmd, char arg[2][64]) {
    int i = 0, j = 0, arg_idx = 0;
    clear(cmd, 16);
    clear(arg[0], 64);
    clear(arg[1], 64);
    while (buf[i] != ' ' && buf[i] != '\0') {
        if (j < 15) {
            cmd[j] = buf[i];
            j++;
        }
        i++;
    }
    cmd[j] = '\0';
    while (buf[i] == ' ') i++;
    while (buf[i] != '\0' && arg_idx < 2) {
        j = 0;
        while (buf[i] != ' ' && buf[i] != '\0') {
            if (j < 63) {
                arg[arg_idx][j] = buf[i];
                j++;
            }
            i++;
        }
        arg[arg_idx][j] = '\0';
        arg_idx++;
        while (buf[i] == ' ') i++;
    }
}
void grandCompany(char* company) {
    if (strcmp(company, "maelstrom")) {
        current_color = 0x04; strcpy(prompt_suffix, "@Storm> ");
    } else if (strcmp(company, "twinadder")) {
        current_color = 0x0E; strcpy(prompt_suffix, "@Serpent> ");
    } else if (strcmp(company, "immortalflames")) {
        current_color = 0x01; strcpy(prompt_suffix, "@Flame> ");
    } else {
        printString("Invalid Grand Company. Use maelstrom, twinadder, or immortalflames.\r\n");
        return;
    }
    setTextColor(current_color);
    clearScreen();
    updatePrompt();
}


void handleCommand(char *cmd, char arg[2][64], char *buf) {
    char out_buf[128];
    int num1, num2, result;
    unsigned int random_val; // ganti nama variabel agar tidak bentrok

    if (strcmp(cmd, "yo")) {
        printString("gurt\r\n");
    } else if (strcmp(cmd, "gurt")) {
        printString("yo\r\n");
    } else if (strcmp(cmd, "yogurt")) {
        // >>> PERUBAHAN 2: Gunakan rand() BUKAN getBiosTick() <<<
        random_val = mod(rand(), 3); 

        if (random_val == 0) {
            printString("gurt> yo\r\n");
        } else if (random_val == 1) {
            printString("gurt> ts unami gng </3\r\n");
        } else { // random_val == 2
            printString("gurt> sygau\r\n");
        }
    } else if (strcmp(cmd, "user")) {
        // ... (sisa fungsi ini sama persis seperti sebelumnya) ...
        if (strlen(arg[0]) > 0) {
            strcpy(username, arg[0]);
            printString("Username changed to ");
            printString(username);
            printString("\r\n");
            updatePrompt();
        }
    } else if (strcmp(cmd, "grandcompany")) {
        if(strlen(arg[0]) > 0) {
            grandCompany(arg[0]);
        } else {
            printString("Please specify a Grand Company.\r\n");
        }
    } else if (strcmp(cmd, "clear")) {
        current_color = 0x07;
        setTextColor(current_color);
        clearScreen();
        strcpy(prompt_suffix, "> ");
        printString("The Grand Companies are saddened by your neutrality.\r\n");
        updatePrompt();
    } else if (strcmp(cmd, "add") || strcmp(cmd, "sub") || strcmp(cmd, "mul") || strcmp(cmd, "div")) {
        if(strlen(arg[0]) == 0 || strlen(arg[1]) == 0) {
            printString("Error: two arguments are required.\r\n");
            return;
        }
        atoi(arg[0], &num1);
        atoi(arg[1], &num2);
        if (strcmp(cmd, "add")) result = num1 + num2;
        if (strcmp(cmd, "sub")) result = num1 - num2;
        if (strcmp(cmd, "mul")) result = num1 * num2;
        if (strcmp(cmd, "div")) {
            if (num2 == 0) {
                printString("Error: Division by zero.\r\n");
                return;
            }
            result = div(num1, num2);
        }
        itoa(result, out_buf);
        printString(out_buf);
        printString("\r\n");
    }
    else {
        if (strlen(cmd) > 0) {
            printString(buf);
            printString("\r\n");
        }
    }
}

int strlen(char* str) {
    int len = 0;
    while(str[len] != '\0') len++;
    return len;
}
