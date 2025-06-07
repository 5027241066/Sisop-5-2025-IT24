# Compiler & Linker settings
CC = bcc
AS = nasm
LD = ld86

# Flags
CFLAGS = -ansi -c -Iinclude
ASFLAGS = -f as86

# Directories
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include

# --- File Definitions ---

# Kumpulan semua file object yang dibutuhkan untuk kernel
OBJECTS = $(BIN_DIR)/kernel.o $(BIN_DIR)/shell.o $(BIN_DIR)/std_lib.o $(BIN_DIR)/kernel_asm.o

# File-file utama
FLOPPY_IMG = $(BIN_DIR)/floppy.img
BOOTLOADER_BIN = $(BIN_DIR)/bootloader.bin
KERNEL_BIN = $(BIN_DIR)/kernel.bin

# --- Targets ---

# Target default: build semuanya
all: build

# Target utama, hanya bergantung pada hasil akhir (floppy.img)
build: $(FLOPPY_IMG)
	@echo "Build successful! Image created at $(FLOPPY_IMG)"

# Cara membuat floppy.img: butuh bootloader dan kernel.bin
$(FLOPPY_IMG): prepare $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo "Writing bootloader and kernel to floppy image..."
	dd if=$(BOOTLOADER_BIN) of=$(FLOPPY_IMG) bs=512 count=1 conv=notrunc > /dev/null 2>&1
	dd if=$(KERNEL_BIN) of=$(FLOPPY_IMG) seek=1 bs=512 conv=notrunc > /dev/null 2>&1

# Cara membuat kernel.bin (linking): butuh semua file .o
$(KERNEL_BIN): $(OBJECTS)
	@echo "Linking kernel files..."
	$(LD) -o $@ -d $^

# Cara membuat file bootloader
$(BOOTLOADER_BIN): $(SRC_DIR)/bootloader.asm
	@echo "Compiling bootloader..."
	$(AS) -f bin $< -o $@

# Aturan umum untuk membuat file .o dari file .c
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -o $@ $<

# Aturan SPESIFIK untuk membuat kernel_asm.o dari kernel.asm
$(BIN_DIR)/kernel_asm.o: $(SRC_DIR)/kernel.asm
	@echo "Compiling src/kernel.asm..."
	$(AS) $(ASFLAGS) -o $@ $<

# Target untuk membuat direktori dan image kosong
prepare:
	@echo "Creating floppy image..."
	@mkdir -p $(BIN_DIR)
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=1024 count=1440 > /dev/null 2>&1

# Target untuk membersihkan file hasil build
clean:
	@echo "Cleaning up..."
	rm -rf $(BIN_DIR)

run:
	bochs -f bochsrc.txt -q

.PHONY: all build prepare clean run
