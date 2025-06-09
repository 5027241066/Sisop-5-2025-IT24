# Sisop-5-2025-IT24

## Kelompok

| No | Nama                   | NRP         |
|----|------------------------|-------------|
| 1  | Aditya Reza Daffansyah | 5027241034  |
| 2  | Ahmad Yafi Ar Rizq     | 5027241066  |
| 3  | Zahra Khaalishah       | 5027241070  |

## Daftar Isi

- [Kelompok](#kelompok)
- [Daftar Isi](#daftar-isi)
- [Soal](#soal)
- [Petunjuk Soal](#petunjuk-soal)
  - [Source](#source)
  - [Headers](#headers)
  - [Makefile](#makefile)
  - [Video Demonstrasi](#video-demonstrasi)
- [Laporan](#laporan)

## Soal

Pada suatu hari, anda merasa sangat lelah dari segala macam praktikum yang sudah ada, sehingga anda berencana untuk tidur yang nyenyak di sebuah jam 3:34AM yang cerah. Tetapi, anda terbangun di dalam dunia berbeda yang bernama "Eorzea". Ada sesuatu yang mengganggu pikiran anda sehingga anda diharuskan membuat sebuah operating system bernama "EorzeOS" untuk mendampingi diri anda dalam dunia ini.

1. Sebagai seorang main character dari dunia ini, ternyata anda memiliki kekuatan yang bernama "The Echo", kekuatan ini memungkinkan anda untuk berbicara pada Operating System ini (mungkin sebenarnya bukan ini kekuatannya, tetapi ini cukup kuat juga), dengan tujuan agar semua hal yang anda katakan, bila bukan merupakan sebuah command yang valid, akan mengulang hal yang anda katakan.

   Ilustrasi:
   ```
   user> Hello!
   Hello!
   user> I have the Echo
   I have the Echo
   ```

2. gurt: yo

   Ilustrasi:
   ```
   user> yo
   gurt
   user> gurt
   yo
   ```

3. Seorang main character memerlukan sebuah nama yang semua orang bisa ingat dengan baik. Buatlah sebuah command yang memungkinkan pengguna untuk mengubah nama user pada shell yang digunakan:
   * `user <username>` = mengubah username menjadi `<username>`
   * `user` = mengubah username menjadi default `user`
   
   Ilustrasi:
   ```
   user> user Tia
   Username changed to Tia
   Tia> user
   Username changed to user
   user>
   ```

4. Tiga negara besar dari Eorzean Alliance butuh bantuan anda untuk ikut serta dalam "Grand Company" mereka sehingga anda bisa mengubah warna terminal ajaib anda sesuai warna utama dari company mereka:
   * `grandcompany maelstrom` = clear terminal, ubah semua teks berikutnya jadi merah
   * `grandcompany twinadder` = clear terminal, ubah semua teks berikutnya jadi kuning
   * `grandcompany immortalflames` = clear terminal, ubah semua teks berikutnya jadi biru
   * `grandcompany <selain atau kosong>` = tunjukkan error message
   * `clear` = clear terminal, ubah semua teks berikutnya kembali jadi awal (para Grand Company sedih kamu netral)

   Selain mengubah seluruh warna terminal, nama anda di dalam terminal akan diberikan tambahan nama judul Grand Company:
   * Maelstrom = `user@Storm`
   * Twin Adder = `user@Serpent`
   * Immortal Flames = `user@Flame`
   * `clear` = menghapus nama grand company

   Ilustrasi:
   ```
   gurt> grandcompany maelstrom
   -- terminal clear menjadi warna merah --
   gurt@Storm> clear
   -- terminal clear menjadi warna putih --
   ```

5. Sebagai pahlawan terkenal di antara ketiga negara besar Eorzean Alliance, salah satu supplier senjata terbesar di seluruh Eorzea bernama "Rowena's House of Splendors" tiba-tiba memerlukan bantuan anda untuk membuat sebuah sistem kalkulator sederhana melalui command karena pemimpin mereka tertidur setelah mengurus semua orang di dalam Eorzea:
   * `add <x> <y>` = x + y
   * `sub <x> <y>` = x - y
   * `mul <x> <y>` = x * y
   * `div <x> <y>` = x / y

   Ilustrasi:
   ```
   user> add 4 2
   6
   user> sub 4 2
   2
   user> mul 3 -2
   -6
   user> div -6 -2
   3
   ```

6. me: yogurt
   
   gurt:
   * `yo`
   * `ts unami gng </3`
   * `sygau`

   pilih secara *random*

   Ilustrasi:
   ```
   user> yogurt
   gurt> yo
   user> yogurt
   gurt> ts unami gng </3
   user> yogurt
   gurt> sygau
   ```

8. Perusahaan mesin "Garlond Ironworks" tiba-tiba lelah mengurus permintaan senjata perang untuk orang ke-148649813234 yang berusaha menghadapi final boss yang sama, sehingga mereka perlu bantuan kamu untuk melengkapi `Makefile` yang diberikan dengan command-command yang sesuai untuk compile seluruh operating system ini.

## Petunjuk Soal

### Source

* [kernel.asm](src/kernel.asm)
  * `_putInMemory`: penjelasan terdapat dalam modul
  * `_interrupt`: fungsi untuk interrupt
    * `number`: interrupt vector number
    * `AX`,`BX`,`CX`,`DX`: register untuk diisi
    * `AX` merupakan kombinasi dari `AH` dan `AL`, pola ini juga berlaku untuk `BX`,`CX`, dan `DX`
    * Untuk menggabungkan jenis register `H` dan `L` menjadi `X` bisa menggunakan salah satu metode berikut:
      ```c
      AX = AH << 8 | AL    // first method
      AX = AH * 256 + AL   // second method
      ```
   * `getBiosTick`: fungsi untuk mendapatkan tick dari BIOS
* [kernel.c](src/kernel.c)
  * Diisi penerapan fungsi `printString`, `readString`, dan `clearScreen` dengan bantuan `kernel.asm`
    * untuk `printString`: Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` dengan parameter `AH = 0x0E` untuk _teletype output_. Karakter yang ditampilkan dapat dimasukkan pada register `AL`. Fungsi ini akan menampilkan string karakter ASCII (_null-terminated_) ke layar.
    * untuk `readString`: Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 16h` dengan parameter `AH = 0x00` untuk _keyboard input_. Fungsi ini akan membaca karakter ASCII (_non-control_) yang dimasukkan oleh pengguna dan menyimpannya pada buffer hingga menekan tombol `Enter`. Handle tombol `Backspace` dibebaskan kepada praktikan.
    * untuk `clearScreen`: Ukuran layar adalah `80x25` karakter. Setelah layar dibersihkan, kursor akan kembali ke posisi awal yaitu `(0, 0)` dan buffer video untuk warna karakter diubah ke warna putih. Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` atau menggunakan fungsi `putInMemory` untuk mengisi memori video.
* [shell.c](src/shell.c)
  * Diisi penerapan shell yang menggunakan fungsi kernel untuk parsing keseluruhan command yang diinput

### Headers

* [std_type.h](include/std_type.h)
  * `byte`: unsigned char data type, untuk angka 0-255 (`0x00`-`0xFF`)
  * `bool`: untuk boolean (true/false), karena boolean tidak built-in
* [std_lib.h](include/std_lib.h)
  * `div`: division
  * `mod`: modulo
  * Pembagian dan modulo tidak ada dalam assembly, sehingga harus dilengkapi dengan operator yang tersedia (`+`,`-`,`*`,`<<`,`>>`)
  * `strcmp`: membandingkan dua string
  * `strcpy`: copy string
  * `clear`: fill memory dengan `0`
  * `atoi`: alphanumeric to integer (string menjadi angka)
  * `itoa`: integer to alphanumeric (angka menjadi string)
* [kernel.h](include/kernel.h)
  * Deklarasi header untuk fungsi-fungsi dalam `kernel.c`
* [shell.h](include/shell.h)
  * Deklarasi header untuk fungsi-fungsi dalam `shell.c`

### Makefile

* [makefile](./makefile)
   * `prepare` : membuat disk image baru `floppy.img` pada direktori `bin/` dengan ukuran 1.44 MB.
   * `bootloader` : mengkompilasi `bootloader.asm` menjadi `bootloader.bin` pada direktori `bin/`.
   * `stdlib` : mengkompilasi `std_lib.c` menjadi `std_lib.o` pada direktori `bin/`.
   * `shell`: mengkompilasi `shell.c` menjadi `shell.o` pada direktori `bin/`.
   * `kernel` : mengkompilasi `kernel.c` menjadi `kernel.o` pada direktori `bin/` dan mengkompilasi `kernel.asm` menjadi `kernel_asm.o` pada direktori `bin/`.
   * `link` : menggabungkan `bootloader.bin`, `kernel.o`, `kernel_asm.o`, dan `std_lib.o` menjadi `floppy.img`.
   * `build` : menjalankan perintah `prepare`, `bootloader`, `stdlib`, `kernel`, dan `link`.

### Video Demonstrasi

[Akses Video dalam Assets](./assets/demo.mp4)

https://github.com/user-attachments/assets/1cfa66b1-b2f5-4e3e-a4b2-ec8b012f6fbb


## Laporan
### 1. The Echo

Command yang tidak dikenal akan di-*echo* kembali ke terminal. Implementasinya cukup dengan:

```c
printString(buf);
printString("\n");
```

### 2. Yo Gurt
Pada soal ini jika user memasukkan input "yo" maka akan keluar "gurt" dan kebalikannya
Untuk mengerjakan soal tersebut code yang digunakan terdapat pada `shell.c`

```
void handleCommand(char *cmd, char arg[2][64], char *buf) {
    char out_buf[128];
    int num1, num2, result;
    unsigned int random_val; 

    if (strcmp(cmd, "yo")) {
        printString("gurt\r\n");
    } else if (strcmp(cmd, "gurt")) {
        printString("yo\r\n");
    }
```

Dimana memiliki fungsi ketika user memasukkan input "yo" maka system akan print "gurt". Kemudian jika user memasukkan input "gurt" maka system akan print "gurt". Input diproses oleh fungsi `parseCommand` yang mengekstrak kata pertama sebagai perintah. Fungsi `strcmp` dari `std_lib.c` membandingkan string. Fungsi `printString` dari `kernel.c` menangani outputnya. Untuk output dari input "yo" dan "gurt" sudah pasti akan sama.

### 3. Update User

Untuk mengganti nama user hal yang pertama dilakukan adalah menginisialisasi dalam fungsi shell:

```
void shell() {

    strcpy(username, "user");  // Mengatur username default
    strcpy(prompt_suffix, "> ");  // Mengatur akhiran prompt default
    updatePrompt();  // Membuat prompt awal
}

```

Ketika pengguna memasukkan perintah user:
Jika terdapat argumen `(strlen(arg[0]) > 0)` maka username baru disalin ke variabel username, kemudian akan di print `Username changed to xxx` sebagai pesan konfirmasi. Username akan diperbarui dengan username yang baru

Jika tidak ada argumen. Secara default, username tetap dipertahankan (meskipun dalam contoh, harusnya kembali menjadi "user")

Fungsi `updatePrompt()` memastikan bahwa shell selalu menampilkan username saat ini dengan cara mengosongkan buffer, menyalin username saat ini, menambahkan akhiran (> )

Ilustrasi:
```
user> user Tia
Username changed to Tia
Tia> user
Username changed to user
user>
```

Implementasi ini menggunakan beberapa fungsi yaitu

- `strcpy` untuk menyalin string
- `strlen` untuk mendapatkan panjang string
- `clear` untuk mengosongkan buffer
- `printString` untuk mencetak output

### 4. Coloring
Dalam file `shell.c` terdapat function `grandCompany` seperti berikut

```
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
```

dan terdapat function `handleCommand`

```
void handleCommand(char *cmd, char arg[2][64], char *buf) {
    } else if (strcmp(cmd, "grandcompany")) {
        if(strlen(arg[0]) > 0) {
            grandCompany(arg[0]);
        } else {
            printString("Please specify a Grand Company.\r\n");
        }
    } else if (strcmp(cmd, "clear")) {
        current_color = 0x07;  // Reset to default white color
        setTextColor(current_color);
        clearScreen();
        strcpy(prompt_suffix, "> ");  // Reset prompt suffix
        printString("The Grand Companies are saddened by your neutrality.\r\n");
        updatePrompt();
    }
}
```

Kemudian untuk coloring terdapat function berikut
```
byte video_attribute = 0x07;  // Default white color

void setTextColor(byte color) {
    video_attribute = color;
}

void clearScreen() {
    int i;
    for (i = 0; i < 80 * 25; i++) {
        putInMemory(0xB800, i * 2, ' ');
        putInMemory(0xB800, i * 2 + 1, video_attribute);
    }
    interrupt(0x10, 0x0200, 0, 0, 0);
}
```

Berikut ini adalah list untuk Color Code:
- 0x04 = Red (Maelstrom)
- 0x0E = Yellow (Twin Adder)
- 0x01 = Blue (Immortal Flames)
- 0x07 = White (Default/Neutral)

Kemudian ketika user memasukkan perintah grandcompany maka perintah tersebut akan diurai dan dikirim ke function grandCompany. Berdasarkan nama company yang dimasukkan, warna yang sesuai diatur melalui variabel current_color. Prompt suffix diatur sesuai (misalnya: @Storm>). Layar dibersihkan (clear) dan warna teks diperbarui. Prompt diperbarui untuk menampilkan afiliasi baru.

Ketika user memasukkan `clear` maka warna diatur ulang menjadi putih (0x07),layar dibersihkan, prompt suffix diatur ulang ke default (>), pesan "saddened" ditampilkan, prompt diperbarui untuk menghapus afiliasi grandcompany. 

Implementasi ini menggunakan beberapa komponen penting:
- `video_attribute` untuk menyimpan warna teks saat ini.
- `prompt_suffix` untuk menyimpan afiliasi company dalam prompt.
- `setTextColor` dan `clearScreen` untuk perubahan tampilan visual.
- `updatePrompt` untuk menjaga tampilan prompt tetap konsisten.

Outputnya akan seperti
```
gurt> grandcompany maelstrom
-- terminal menjadi warna merah --
gurt@Storm> clear
-- terminal menjadi warna putih --
```

### 5. Kalkulator
Command:

```
add <x> <y> → penjumlahan

sub <x> <y> → pengurangan

mul <x> <y> → perkalian

div <x> <y> → pembagian

```
Implementasi di handleCommand:
```
num1 = atoi(arg[0]);
num2 = atoi(arg[1]);

if (strcmp(cmd, "add")) {
    result = num1 + num2;
} else if (strcmp(cmd, "sub")) {
    result = num1 - num2;
} else if (strcmp(cmd, "mul")) {
    result = num1 * num2;
} else if (strcmp(cmd, "div")) {
    if (num2 == 0) {
        printString("Division by zero error\r\n");
    } else {
        result = num1 / num2;
    }
}
itoa(result, out_buf);
printString(out_buf); printString("\r\n");

```

### 6. Random
Pada soal ini ketika type "yogurt" maka akan muncul pesan random berikut "yo", "ts unami gng </3", "sygau". 

Untuk menjalankan soal berikut, terdapat pada function `handleCommand`

```
void handleCommand(char *cmd, char arg[2][64], char *buf) {
    // ......
    else if (strcmp(cmd, "yogurt")) {
        random_val = mod(rand(), 3); 

        if (random_val == 0) {
            printString("gurt> yo\r\n");
        } else if (random_val == 1) {
            printString("gurt> ts unami gng </3\r\n");
        } else { 
            printString("gurt> sygau\r\n");
        }
    }
    // ......
}
```

Selain itu, terdapat support juga dari file std_lib.c
```
static unsigned int rand_seed;

void srand(unsigned int seed) {
    rand_seed = seed;
}

unsigned int rand() {
    rand_seed = rand_seed * 25173 + 13849;
    return rand_seed;
}

int mod(int a, int b) {
    if (b == 0) return -1;
    if (b < 0) b = -b;
    while (a >= b) {
        a = a - b;
    }
    return a;
}
```

Kemudian untuk pemilihan random terdapat pada file shell:
```
void shell() {
    // ......
    srand(getBiosTick());  // Initialize random seed using BIOS tick count
    // ......
}
```

Untuk pemilihan random nilai awal diatur dengan `getBiosTick()`, yang mengambil jumlah tick BIOS yang memastikan urutan angka berbeda tiap kali OS dijalankan.

Ketika user menulis yogurt, perintah dikenali oleh `strcmp(cmd, "yogurt")`. Fungsi `rand()` digunakan untuk menghasilkan angka acak. `mod(rand(), 3)` membatasi hasil menjadi angka antara 0 hingga 2. Berdasarkan angka tersebut, salah satu dari tiga respons berikut dipilih:
Berdasarkan angka tersebut, salah satu dari tiga respons berikut dipilih:
- 0: "gurt> yo"
- 1: "gurt> ts unami gng </3"
- 2: "gurt> sygau"

Dengan mekanisme ini, setiap kali perintah yogurt dipanggil, pengguna akan mendapatkan salah satu dari tiga respons yang berbeda secara acak. 
> Isi sesuai pengerjaan.
