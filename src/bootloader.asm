; src/bootloader.asm
bits 16          ; Kita bekerja dalam mode real 16-bit

ORG 0x7C00       ; Alamat standar di mana BIOS me-load bootloader

KERNEL_LOAD_SEGMENT equ 0x1000  ; Segmen memori tempat kernel akan di-load
KERNEL_LOAD_OFFSET  equ 0x0000  ; Offset dalam segmen tersebut
KERNEL_START_SECTOR equ 1       ; Kernel dimulai dari LBA sector 1 (sector ke-2 di disk)
NUM_SECTORS_TO_LOAD equ 15      ; Jumlah maksimum sector kernel yang akan di-load

start:
    cli            ; Matikan interrupt
    
    ; Setup segment registers awal
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Setup stack pointer
    mov ax, 0x0700
    mov ss, ax
    mov sp, 0xFFFF

    sti        ; Aktifkan kembali interrupt

    ; --- Load Kernel dari Disk ---
    mov ax, KERNEL_LOAD_SEGMENT
    mov es, ax                  ; ES:BX buffer tujuan: KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET
    mov bx, KERNEL_LOAD_OFFSET

    mov ah, 0x02                ; BIOS: Read Sectors
    mov al, NUM_SECTORS_TO_LOAD ; Jumlah sector
    mov ch, 0                   ; Cylinder 0
    mov dh, 0                   ; Head 0
    mov cl, KERNEL_START_SECTOR + 1 ; Sector number (LBA 1 -> CHS Sector 2)
    mov dl, 0x00                ; Drive A: (floppy)

    int 0x13                    ; Panggil BIOS disk service
    jc disk_error               ; Jika error, lompat ke disk_error

    ; --- Persiapan untuk Melompat ke Kernel ---
    cli ; Matikan interrupt sebelum melompat

    ; Lompat ke alamat entry point kernel
    jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

disk_error:
    mov si, error_msg
    call print_string_simple
error_loop:
    jmp error_loop

; Fungsi print sederhana untuk error
print_string_simple:
    mov ah, 0x0E        ; Teletype output
print_char_simple:
    lodsb               ; Ambil karakter dari [SI] ke AL, SI++
    or al, al           ; Cek apakah AL = 0 (null terminator)
    jz print_done_simple
    int 0x10            ; Cetak karakter
    jmp print_char_simple
print_done_simple:
    ret

error_msg: db 'Disk read error!', 0

    times 510 - ($-$$) db 0 ; Padding
    dw 0xAA55             ; Boot signature
