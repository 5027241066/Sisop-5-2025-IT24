#include "kernel.h" // Asumsikan ini mendeklarasikan:
                    // extern int interrupt(int number, int AX, int BX, int CX, int DX);
                    // extern void putInMemory(int segment, int address, char character);
                    // void clearScreen();
                    // void printString(char *str);
                    // void readString(char *buf);
                    // Mungkin juga: void setKernelPrintAttribute(byte attribute); (akan kita tambahkan)
#include "shell.h"  // Untuk memanggil shell()
#include "std_type.h" // Untuk byte

// Definisi untuk layar dan atribut default
#define VIDEO_MEMORY_SEGMENT 0xB800 // Alamat segmen memori video
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_ATTRIBUTE 0x07 // Atribut default: Putih pada Hitam

// Variabel global statis untuk menyimpan atribut teks saat ini
static byte g_current_text_attribute = DEFAULT_ATTRIBUTE;

// Fungsi untuk mengatur atribut teks yang akan digunakan oleh printString dan clearScreen
// Ini perlu dideklarasikan di kernel.h jika akan dipanggil dari shell.c
void setKernelPrintAttribute(byte attribute) {
    g_current_text_attribute = attribute;
}

// Helper untuk manajemen kursor (internal kernel.c)
static int g_cursor_row = 0;
static int g_cursor_col = 0;

void update_cursor_bios() {
    // int 10h, AH=0x02 (Set cursor position), BH=page (0), DH=row, DL=col
    interrupt(0x10, (0x02 << 8), 0x00, (g_cursor_row << 8) | g_cursor_col, 0);
}

void scroll_screen_if_needed() {
    if (g_cursor_row >= SCREEN_HEIGHT) {
        // Scroll satu baris ke atas menggunakan int 10h, AH=0x06
        // AL = 1 (jumlah baris scroll)
        // BH = atribut untuk baris baru yang kosong (g_current_text_attribute)
        // CH,CL = Y,X kiri atas area scroll (0,0)
        // DH,DL = Y,X kanan bawah area scroll (SCREEN_HEIGHT-1, SCREEN_WIDTH-1)
        interrupt(0x10, 
                  (0x06 << 8) | 0x01,                         // AH=0x06, AL=1
                  (g_current_text_attribute << 8),            // BH = attribute
                  0x0000,                                     // CH=0, CL=0
                  ((SCREEN_HEIGHT - 1) << 8) | (SCREEN_WIDTH - 1) // DH, DL
        );
        g_cursor_row = SCREEN_HEIGHT - 1; // Setelah scroll, kursor di baris terakhir
        g_cursor_col = 0; // Biasanya kursor kembali ke awal baris setelah scroll
    }
}

// Mencetak satu karakter ke layar dengan atribut saat ini
// Menggunakan int 10h, AH=0x09 (Write Character and Attribute)
// dan mengelola posisi kursor secara manual.
void printChar_with_attribute(char c) {
    if (c == '\n') {
        g_cursor_col = 0;
        g_cursor_row++;
    } else if (c == '\r') {
        g_cursor_col = 0; // Carriage return
    } else if (c == '\b') {
        if (g_cursor_col > 0) {
            g_cursor_col--;
            // Tulis spasi dengan atribut saat ini untuk "menghapus"
            interrupt(0x10, (0x09 << 8) | ' ', (0x00 << 8) | g_current_text_attribute, 1, 0); 
            // Mundurkan kursor lagi secara visual (meskipun sudah di g_cursor_col)
            // Sebenarnya, update_cursor_bios() di akhir readString akan menanganinya.
            // Untuk efek visual backspace di readString, kita akan update cursor di sana.
        } else if (g_cursor_row > 0) { // Backspace dari awal baris
            g_cursor_row--;
            g_cursor_col = SCREEN_WIDTH - 1;
            interrupt(0x10, (0x09 << 8) | ' ', (0x00 << 8) | g_current_text_attribute, 1, 0);
        }
    } else {
        // int 10h, AH=0x09 (Write char and attribute)
        // AL = char, BH = page (0), BL = attribute, CX = count (1)
        interrupt(0x10, (0x09 << 8) | c, (0x00 << 8) | g_current_text_attribute, 1, 0);
        g_cursor_col++;
        if (g_cursor_col >= SCREEN_WIDTH) {
            g_cursor_col = 0;
            g_cursor_row++;
        }
    }
    scroll_screen_if_needed();
    update_cursor_bios();
}

// Fungsi yang diminta di struktur awalmu
int main() {
    setKernelPrintAttribute(DEFAULT_ATTRIBUTE); // Set atribut awal
    clearScreen(); // Akan menggunakan g_current_text_attribute (default)
    // printString("Welcome from kernel.c main!\n"); // Pesan selamat datang awal
    shell(); // Langsung panggil shell
    
    // Seharusnya tidak pernah sampai sini jika shell() adalah loop tak terbatas
    while(true);
    return 0;
}

void printString(char *str) {
    int i = 0;
    // Dapatkan posisi kursor awal jika kita tidak mau mengelolanya secara global terus menerus
    // Untuk implementasi ini, printChar_with_attribute sudah mengelola kursor global g_cursor_row/col
    while (str[i] != '\0') {
        printChar_with_attribute(str[i]);
        i++;
    }
}

void readString(char *buf) {
    int i = 0;
    char c;
    unsigned short ax_val_interrupt;

    // Batasi panjang buffer (misal 128 - 1 untuk null terminator)
    // Ukuran buf tidak diketahui di sini, jadi kita pakai batas aman.
    // Sebaiknya readString menerima parameter size juga.
    // Untuk sekarang, asumsikan buf cukup besar (misal 128 dari shell.c)
    int buffer_capacity = 127; 

    while (i < buffer_capacity) {
        ax_val_interrupt = interrupt(0x16, (0x00 << 8), 0, 0, 0); // AH=0x00 (read key)
        c = (char)(ax_val_interrupt & 0xFF); // AL = ASCII character

        if (c == '\r') { // Tombol Enter (BIOS biasanya mengirim \r)
            // printChar_with_attribute('\n'); // Shell akan print newline setelah command
            break; 
        } else if (c == '\n') { // Jika sistem mengirim \n untuk Enter (jarang dari BIOS direct read)
            // printChar_with_attribute('\n');
            break;
        } else if (c == '\b') { // Tombol Backspace (ASCII 0x08)
            if (i > 0) {
                i--;
                // Mundurkan kursor, cetak spasi, mundurkan kursor lagi
                // Ini efek visual backspace
                if (g_cursor_col > 0) {
                    g_cursor_col--;
                } else if (g_cursor_row > 0) {
                    g_cursor_row--;
                    g_cursor_col = SCREEN_WIDTH - 1;
                }
                update_cursor_bios(); // Pindahkan kursor BIOS dulu
                printChar_with_attribute(' '); // Cetak spasi (akan memajukan g_cursor_col)
                // Setelah printChar(' '), g_cursor_col maju lagi, kita perlu mundurkan lagi ke posisi spasi.
                if (g_cursor_col > 0) { // Mundurkan g_cursor_col ke posisi spasi
                    g_cursor_col--;
                } else if (g_cursor_row > 0) { // Jika di awal baris, g_cursor_col akan jadi SCREEN_WIDTH-1
                    // Ini sudah benar jika printChar(' ') tidak newline.
                    // Jika printChar(' ') membuat newline, logika ini perlu disesuaikan.
                    // Asumsi printChar(' ') tidak membuat newline jika g_cursor_col < SCREEN_WIDTH
                }
                 update_cursor_bios(); // Update kursor BIOS ke posisi spasi yang baru ditulis
            }
        } else if (c >= ' ' && c <= '~') { // Karakter yang bisa dicetak
            buf[i] = c;
            printChar_with_attribute(c); // Echo karakter ke layar
            i++;
        }
        // Karakter kontrol lain diabaikan
    }
    buf[i] = '\0'; // Null-terminate string
}

void clearScreen() {
    // Fungsi ini sekarang akan membersihkan layar dengan g_current_text_attribute
    int r, c_loop; // g_cursor_col adalah variabel global, gunakan nama lain untuk loop
    g_cursor_row = 0;
    g_cursor_col = 0;
    for (r = 0; r < SCREEN_HEIGHT; ++r) {
        for (c_loop = 0; c_loop < SCREEN_WIDTH; ++c_loop) {
            // Langsung gunakan putInMemory untuk efisiensi dan menghindari kompleksitas kursor printChar
            putInMemory(VIDEO_MEMORY_SEGMENT, (r * SCREEN_WIDTH + c_loop) * 2, ' ');
            putInMemory(VIDEO_MEMORY_SEGMENT, (r * SCREEN_WIDTH + c_loop) * 2 + 1, g_current_text_attribute);
        }
    }
    update_cursor_bios(); // Set kursor ke (0,0) setelah clear
}
