#include "std_lib.h" // Pastikan path include sudah benar ("std_lib.h")

// Fungsi pembagian (div)
// Implementasi sederhana, belum menangani overflow secara spesifik.
int div(int a, int b) {
    int result = 0;
    int sign = 1;

    if (b == 0) {
        // Pembagian dengan nol. Di OS nyata ini bisa memicu exception.
        // Untuk sekarang, return 0.
        // Kamu mungkin ingin menambahkan cara error handling yang lebih baik.
        return 0; 
    }

    if (a < 0) {
        sign *= -1;
        a = -a; // Jadikan positif untuk perhitungan
    }
    if (b < 0) {
        sign *= -1;
        b = -b; // Jadikan positif untuk perhitungan
    }

    while (a >= b) {
        a -= b;
        result++;
    }
    return sign * result;
}

// Fungsi modulus (mod)
// a mod b = a - (b * div(a,b))
// Hasilnya memiliki tanda yang sama dengan 'a' (sesuai C99 untuk operator %).
int mod(int a, int b) {
    if (b == 0) {
        return 0; // Atau error, konsisten dengan div
    }

    // Jika b negatif, jadikan positif untuk perhitungan internal div,
    // tapi tanda asli 'a' yang menentukan tanda hasil mod.
    // Gunakan b_abs untuk div agar perilakunya konsisten
                                    // atau gunakan div(a,b) jika div(a,b) sudah menangani tanda dengan benar
                                    // untuk a - (a/b)*b

    // Mengikuti C99: a % b = a - (a/b)*b. Tanda hasil mengikuti 'a'.
    // Jika 'b' negatif, (a/b) akan berbeda.
    // Cara paling aman adalah menggunakan sifat matematis:
    // a = q*b + r, dengan 0 <= |r| < |b|, dan r memiliki tanda yang sama dengan a.

    int remainder = a;
    if (a >= 0 && b > 0) { // a+, b+
        while (remainder >= b) remainder -= b;
    } else if (a >= 0 && b < 0) { // a+, b-
        while (remainder >= -b) remainder -= (-b); // remainder -= b_abs
    } else if (a < 0 && b > 0) { // a-, b+
        while (remainder <= -b) remainder += b; // remainder += b_abs
    } else { // a < 0 && b < 0 -> a-, b-
        while (remainder <= b) remainder += (-b); // remainder += b_abs
    }
    // Penyesuaian akhir berdasarkan definisi umum:
    // jika a dan b berbeda tanda dan remainder != 0, maka remainder - b (jika a positif) atau remainder + b (jika a negatif)
    // Namun, implementasi yang lebih sederhana adalah:
    // remainder = a - div(a,b) * b;  (Ini memerlukan div(a,b) yang benar-benar matematis)
    // Untuk implementasi sederhana:
    int val_a = a;
    int val_b = (b < 0) ? -b : b; // b absolut
    
    if (val_a < 0) val_a = -val_a;

    while(val_a >= val_b) {
        val_a -= val_b;
    }

    return (a < 0) ? -val_a : val_a; // Tanda hasil mengikuti 'a'
}


// Fungsi perbandingan string (strcmp)
// Mengembalikan true jika sama, false jika berbeda.
bool strcmp(char *str1, char *str2) {
    while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    // Sama jika kedua string berakhir pada saat yang sama dan semua karakter sebelumnya sama
    return (*str1 == *str2); 
}

// Fungsi penyalinan string (strcpy)
void strcpy(char *dst, char *src) {
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0'; // Null-terminate string tujuan
}

// Fungsi pembersihan buffer (clear)
// Mengisi buffer dengan 0.
void clear(byte *buf, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

// Fungsi konversi string ke integer (atoi)
// Mendukung angka negatif. Hasil disimpan ke *num.
// Sesuai signature void atoi(char *str, int *num)
void atoi(char *str, int *num) {
    int res = 0;
    int sign = 1;
    int i = 0;

    // Lewati spasi di awal (opsional, tapi umum)
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    // Tangani tanda negatif atau positif
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Iterasi melalui digit
    for (; str[i] != '\0'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            res = res * 10 + (str[i] - '0');
        } else {
            break; // Karakter bukan digit, hentikan parsing
        }
    }
    *num = sign * res; // Simpan hasil ke alamat yang ditunjuk oleh num
}

// Fungsi konversi integer ke string (itoa)
// Mendukung angka negatif.
void itoa(int num, char *str) {
    int i = 0;
    bool isNegative = false;
    int start, end;
    char temp;

    // Tangani kasus 0 secara eksplisit
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Tangani angka negatif
    if (num < 0) {
        isNegative = true;
        // Untuk kasus INT_MIN (-2147483648), -num akan overflow jika int 32-bit.
        // Cara aman: proses sebagai positif, tambahkan tanda di akhir.
        // Atau, jika num adalah INT_MIN, tangani secara khusus atau gunakan unsigned untuk proses.
        // Untuk OS sederhana, kita asumsikan tidak mencapai INT_MIN atau biarkan overflow.
        num = -num; 
    }

    // Konversi angka menjadi string (urutan terbalik)
    while (num != 0) {
        int rem = num % 10; // Gunakan % bukan fungsi mod() kita untuk simple digit extraction
        str[i++] = rem + '0'; // Konversi digit ke karakter ASCII
        num = num / 10; // Gunakan / bukan fungsi div() kita
    }

    // Tambahkan tanda negatif jika perlu
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate string

    // Balik urutan string
    start = 0;
    end = i - 1;
    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
