# Makefile untuk EorzeOS

# --- Konfigurasi Compiler dan Assembler ---
CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy

# --- Direktori ---
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include

# --- Nama File Output ---
FLOPPY_IMG = $(BIN_DIR)/floppy.img
BOOTLOADER_BIN = $(BIN_DIR)/bootloader.bin
KERNEL_C_O = $(BIN_DIR)/kernel.o
KERNEL_ASM_O = $(BIN_DIR)/kernel_asm.o
STDLIB_O = $(BIN_DIR)/std_lib.o
SHELL_O = $(BIN_DIR)/shell.o
# File antara untuk kernel yang sudah di-link sebelum menjadi .bin
KERNEL_ELF = $(BIN_DIR)/kernel.elf
# File kernel final dalam format binary
KERNEL_BIN = $(BIN_DIR)/kernel.bin

# --- Flags ---
# CFLAGS:
# -m32: Target arsitektur 32-bit (sesuaikan jika perlu)
# -c: Compile saja, jangan link
# -std=c99: Standar C99
# -Wall -Wextra: Tampilkan semua warning
# -nostdlib: Jangan gunakan library standar C (karena kita buat sendiri)
# -nostdinc: Jangan gunakan include standar C
# -fno-builtin: Jangan gunakan fungsi built-in compiler
# -fno-stack-protector: Matikan proteksi stack
# -I$(INCLUDE_DIR): Tambahkan direktori 'include' untuk pencarian header
# -g: Tambahkan debug symbols
CFLAGS = -m32 -c -std=c99 -Wall -Wextra -nostdlib -nostdinc -fno-builtin -fno-stack-protector -I$(INCLUDE_DIR) -g

# ASFLAGS_BOOT: untuk bootloader (format binary)
ASFLAGS_BOOT = -f bin
# ASFLAGS_KERNEL: untuk kernel assembly (format ELF agar bisa di-link dengan C)
ASFLAGS_KERNEL = -f elf32 # atau -f elf jika nasm versi baru bisa otomatis 32-bit

# --- Targets ---

# Target default: build semua
all: build

# Target prepare: Membuat direktori bin dan image floppy kosong 1.44MB
prepare:
	@echo "==> Membuat direktori $(BIN_DIR)..."
	@mkdir -p $(BIN_DIR)
	@echo "==> Membuat image floppy $(FLOPPY_IMG)..."
	@dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880 status=none

# Target bootloader: Mengkompilasi bootloader.asm menjadi bootloader.bin
bootloader: $(BOOTLOADER_BIN)

$(BOOTLOADER_BIN): $(SRC_DIR)/bootloader.asm  # <-- Penyesuaian di sini
	@echo "==> Mengkompilasi bootloader: $< -> $@"
	@$(AS) $(ASFLAGS_BOOT) $< -o $@

# Target stdlib: Mengkompilasi std_lib.c menjadi std_lib.o
stdlib: $(STDLIB_O)

$(STDLIB_O): $(SRC_DIR)/std_lib.c $(wildcard $(INCLUDE_DIR)/*.h)
	@echo "==> Mengkompilasi Pustaka Standar: $< -> $@"
	@$(CC) $(CFLAGS) $< -o $@

# Target shell: Mengkompilasi shell.c menjadi shell.o
shell: $(SHELL_O)

$(SHELL_O): $(SRC_DIR)/shell.c $(wildcard $(INCLUDE_DIR)/*.h)
	@echo "==> Mengkompilasi Shell: $< -> $@"
	@$(CC) $(CFLAGS) $< -o $@

# Target kernel: Mengkompilasi kernel.c dan kernel.asm
kernel: $(KERNEL_C_O) $(KERNEL_ASM_O)

$(KERNEL_C_O): $(SRC_DIR)/kernel.c $(wildcard $(INCLUDE_DIR)/*.h)
	@echo "==> Mengkompilasi Kernel C: $< -> $@"
	@$(CC) $(CFLAGS) $< -o $@

$(KERNEL_ASM_O): $(SRC_DIR)/kernel.asm
	@echo "==> Mengkompilasi Kernel Assembly: $< -> $@"
	@$(AS) $(ASFLAGS_KERNEL) $< -o $@

# Target internal untuk membuat kernel.bin dari semua file .o
# $^ adalah variabel otomatis Makefile yang berarti "semua prerequisites"
$(KERNEL_BIN): $(KERNEL_C_O) $(KERNEL_ASM_O) $(STDLIB_O) $(SHELL_O)
	@echo "==> Me-link semua komponen kernel menjadi $(KERNEL_ELF)..."
	@$(LD) -m elf_i386 -Ttext 0x1000 -e main -o $(KERNEL_ELF) $^
	@echo "==> Mengkonversi $(KERNEL_ELF) menjadi $(KERNEL_BIN)..."
	@$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

# Target link: Menggabungkan bootloader.bin dan kernel.bin ke floppy.img
# Ini mengasumsikan floppy.img sudah dibuat oleh 'prepare'
link: prepare $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo "==> Menulis $(BOOTLOADER_BIN) ke $(FLOPPY_IMG)..."
	@dd if=$(BOOTLOADER_BIN) of=$(FLOPPY_IMG) conv=notrunc status=none
	@echo "==> Menulis $(KERNEL_BIN) ke $(FLOPPY_IMG) (mulai dari sector 1)..."
	@dd if=$(KERNEL_BIN) of=$(FLOPPY_IMG) seek=1 conv=notrunc status=none


# Target build: Menjalankan semua langkah yang diperlukan
build: prepare bootloader stdlib shell kernel link # 'link' sudah mencakup pembuatan KERNEL_BIN
	@echo "==> Build EorzeOS Selesai! Image ada di $(FLOPPY_IMG)"

# Target run (opsional): Menjalankan Bochs
# Mengasumsikan bochsrc.txt ada di root direktori proyek
run: $(FLOPPY_IMG)
	@echo "==> Menjalankan EorzeOS dengan Bochs..."
	@bochs -f bochsrc.txt -q # -q untuk quiet mode, hapus jika ingin lihat output Bochs

# Target clean: Membersihkan file hasil kompilasi
clean:
	@echo "==> Membersihkan file build..."
	@rm -rf $(BIN_DIR) # Hapus direktori bin
	@rm -f $(FLOPPY_IMG) # Hapus floppy image jika ada di root (seharusnya di bin, tapi untuk jaga-jaga)


# Deklarasi target palsu (tidak menghasilkan file dengan nama target)
.PHONY: all prepare bootloader stdlib shell kernel link build run clean
