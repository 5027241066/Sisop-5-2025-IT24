#include "shell.h"    // Deklarasi fungsi shell
#include "kernel.h"   // Untuk printString, readString, clearScreen, setKernelPrintAttribute, getBiosTick
#include "std_lib.h"  // Untuk strcmp, strcpy, atoi, itoa, div, mod, clear

// Definisi warna (sesuaikan dengan standar atribut CGA/VGA Text Mode)
// Format: 0xBF (B=Background, F=Foreground)
// Contoh:
#define ATTR_DEFAULT        0x07 // Foreground: Putih (7), Background: Hitam (0)
#define ATTR_MAELSTROM_RED  0x04 // Foreground: Merah (4), Background: Hitam (0)
#define ATTR_TWINADDER_YLW  0x0E // Foreground: Kuning (E), Background: Hitam (0)
#define ATTR_IMMORTAL_BLUE  0x01 // Foreground: Biru (1), Background: Hitam (0) 
                                 // (Biru gelap, 0x09 untuk biru terang)

// Variabel global untuk shell (file scope)
char g_current_username[64];
char g_grand_company_suffix[10]; // e.g., "@Storm"

void shell() {
    char input_buffer[128];    // Buffer untuk input pengguna
    char command[64];          // Buffer untuk menyimpan perintah yang diparsing
    char arguments[2][64];     // Buffer untuk menyimpan argumen (maks 2 argumen)

    // Inisialisasi state shell
    strcpy(g_current_username, "user"); // Nama user default
    g_grand_company_suffix[0] = '\0';   // Suffix Grand Company kosong di awal
    
    // Atribut awal sudah di-set oleh kernel.main() saat memanggil clearScreen() pertama kali.
    // Pesan selamat datang sudah dicetak oleh kernel.main()

    while (true) {
        // Tampilkan prompt
        printString(g_current_username);
        printString(g_grand_company_suffix);
        printString("> ");

        // Baca input dari pengguna
        readString(input_buffer);
        printString("\n"); // Baris baru setelah input dibaca, sebelum output perintah

        // Bersihkan buffer command dan arguments sebelum parsing baru
        clear((byte*)command, 64);
        clear((byte*)arguments[0], 64);
        clear((byte*)arguments[1], 64);

        // Parse perintah dari input_buffer
        parseCommand(input_buffer, command, arguments);

        // Proses perintah
        if (strcmp(command, "user")) {
            if (arguments[0][0] == '\0') { // Perintah 'user' tanpa argumen
                strcpy(g_current_username, "user");
                printString("Username changed to user\n");
            } else { // Perintah 'user <username_baru>'
                strcpy(g_current_username, arguments[0]);
                printString("Username changed to ");
                printString(g_current_username);
                printString("\n");
            }
        } else if (strcmp(command, "grandcompany")) {
            bool gc_updated = false;
            byte new_attribute = ATTR_DEFAULT; // Default jika argumen tidak valid

            if (strcmp(arguments[0], "maelstrom")) {
                new_attribute = ATTR_MAELSTROM_RED;
                strcpy(g_grand_company_suffix, "@Storm");
                gc_updated = true;
            } else if (strcmp(arguments[0], "twinadder")) {
                new_attribute = ATTR_TWINADDER_YLW;
                strcpy(g_grand_company_suffix, "@Serpent");
                gc_updated = true;
            } else if (strcmp(arguments[0], "immortalflames")) {
                new_attribute = ATTR_IMMORTAL_BLUE;
                strcpy(g_grand_company_suffix, "@Flame");
                gc_updated = true;
            } else {
                printString("Error: Invalid Grand Company or missing argument.\n");
            }

            if (gc_updated) {
                setKernelPrintAttribute(new_attribute); // Beritahu kernel untuk menggunakan atribut baru
                clearScreen(); // Kernel akan membersihkan layar dengan atribut baru tersebut
                // Pesan konfirmasi akan dicetak dengan warna baru
                // printString("Grand Company allegiance updated.\n"); 
            }
        } else if (strcmp(command, "clear")) {
            setKernelPrintAttribute(ATTR_DEFAULT); // Kembalikan ke atribut default
            g_grand_company_suffix[0] = '\0';      // Hapus suffix Grand Company
            clearScreen(); // Kernel akan membersihkan layar dengan atribut default
            // printString("Terminal cleared to default.\n");
        } else if (strcmp(command, "add")) {
            int num1, num2;
            atoi(arguments[0], &num1); // Menggunakan atoi dengan signature void(char*, int*)
            atoi(arguments[1], &num2);
            int result = num1 + num2;
            char result_str[32]; // Buffer untuk string hasil
            itoa(result, result_str);
            printString(result_str);
            printString("\n");
        } else if (strcmp(command, "sub")) {
            int num1, num2;
            atoi(arguments[0], &num1);
            atoi(arguments[1], &num2);
            int result = num1 - num2;
            char result_str[32];
            itoa(result, result_str);
            printString(result_str);
            printString("\n");
        } else if (strcmp(command, "mul")) {
            int num1, num2;
            atoi(arguments[0], &num1);
            atoi(arguments[1], &num2);
            int result = num1 * num2;
            char result_str[32];
            itoa(result, result_str);
            printString(result_str);
            printString("\n");
        } else if (strcmp(command, "div")) {
            int num1, num2;
            atoi(arguments[0], &num1);
            atoi(arguments[1], &num2);
            if (num2 == 0) {
                printString("Error: Division by zero.\n");
            } else {
                int result = div(num1, num2); // Menggunakan fungsi div kustom
                char result_str[32];
                itoa(result, result_str);
                printString(result_str);
                printString("\n");
            }
        } else if (strcmp(command, "yo")) {
            printString("gurt\n");
        } else if (strcmp(command, "gurt")) {
            printString("yo\n");
        } else if (strcmp(command, "yogurt")) {
            unsigned int tick = getBiosTick(); // Panggil fungsi dari kernel.asm via kernel.c
            int choice = mod(tick, 4);       // Gunakan mod dari std_lib.c

            if (choice == 0) printString("gurt\n");
            else if (choice == 1) printString("yo\n");
            else if (choice == 2) printString("ts unami gng </3\n");
            else printString("sygau\n");
        }
        // "The Echo" Power: Jika command tidak kosong tapi tidak dikenali,
        // atau jika input buffer tidak kosong tapi tidak menghasilkan command (misal hanya spasi).
        else if (input_buffer[0] != '\0' && input_buffer[0] != '\r' && input_buffer[0] != '\n') {
            // Echo seluruh input buffer jika itu bukan perintah yang valid atau hanya spasi
             printString(input_buffer); 
             printString("\n");
        }
        // Jika input buffer kosong (langsung enter), command[0] akan '\0', tidak ada echo, loop lanjut.
    }
}

// Fungsi untuk mem-parsing perintah dan argumennya
// Format sederhana: cmd arg1 arg2 (dipisahkan spasi atau tab)
void parseCommand(char *buf, char *command, char arguments[2][64]) {
    int i = 0; // Indeks untuk buffer input (buf)
    int j = 0; // Indeks untuk buffer output (cmd atau arg)

    // Langkah 1: Lewati spasi/tab di awal buffer input
    while (buf[i] == ' ' || buf[i] == '\t') {
        i++;
    }

    // Jika setelah spasi/tab hanya ada akhir baris atau null, berarti tidak ada command
    if (buf[i] == '\r' || buf[i] == '\n' || buf[i] == '\0') {
        command[0] = '\0'; // Set command kosong
        arguments[0][0] = '\0'; // Set arg1 kosong
        arguments[1][0] = '\0'; // Set arg2 kosong
        return;
    }

    // Langkah 2: Ekstrak command
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\r' && buf[i] != '\n' && buf[i] != '\0') {
        if (j < 63) { // Hindari overflow buffer command
            command[j++] = buf[i];
        }
        i++;
    }
    command[j] = '\0'; // Null-terminate string command

    // Langkah 3: Lewati spasi/tab setelah command
    while (buf[i] == ' ' || buf[i] == '\t') {
        i++;
    }

    // Jika setelah spasi/tab hanya ada akhir baris atau null, berarti tidak ada argumen
    if (buf[i] == '\r' || buf[i] == '\n' || buf[i] == '\0') {
        arguments[0][0] = '\0'; // Set arg1 kosong
        arguments[1][0] = '\0'; // Set arg2 kosong
        return;
    }

    // Langkah 4: Ekstrak argumen pertama
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\r' && buf[i] != '\n' && buf[i] != '\0') {
        if (j < 63) { // Hindari overflow buffer arguments[0]
            arguments[0][j++] = buf[i];
        }
        i++;
    }
    arguments[0][j] = '\0'; // Null-terminate string argumen pertama

    // Langkah 5: Lewati spasi/tab setelah argumen pertama
    while (buf[i] == ' ' || buf[i] == '\t') {
        i++;
    }

    // Jika setelah spasi/tab hanya ada akhir baris atau null, berarti tidak ada argumen kedua
    if (buf[i] == '\r' || buf[i] == '\n' || buf[i] == '\0') {
        arguments[1][0] = '\0'; // Set arg2 kosong
        return;
    }

    // Langkah 6: Ekstrak argumen kedua
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\r' && buf[i] != '\n' && buf[i] != '\0') {
        if (j < 63) { // Hindari overflow buffer arguments[1]
            arguments[1][j++] = buf[i];
        }
        i++;
    }
    arguments[1][j] = '\0'; // Null-terminate string argumen kedua
}
