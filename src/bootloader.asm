; bootloader.asm
bits 16

KERNEL_SEGMENT equ 0x1000 ; Kernel akan dimuat di 0x1000:0x0000
KERNEL_SECTORS equ 15     ; Alokasi 15 sektor untuk kernel

; bootloader code
bootloader:
    ; Muat kernel ke memori
    mov ax, KERNEL_SEGMENT      ; Alamat tujuan kernel
    mov es, ax                  ; Atur Extra Segment (ES) untuk tujuan
    mov bx, 0x0000              ; Alamat offset di ES

    ; Panggil interrupt BIOS untuk membaca disk
    mov ah, 0x02                ; Service 0x02: Read disk sectors
    mov al, KERNEL_SECTORS      ; Jumlah sektor yang akan dibaca
    mov ch, 0                   ; Cylinder: 0 (track terluar)
    mov cl, 2                   ; Sector: 2 (sektor pertama SETELAH bootloader)
    mov dh, 0                   ; Head: 0
    mov dl, 0x00                ; Drive: 0 (A:)
    int 0x13                    ; Panggil BIOS disk service

    ; Jika terjadi error saat read (carry flag set), lompat ke loop tak terbatas
    jc failed_to_load

    ; Set up segment registers untuk kernel
    mov ax, KERNEL_SEGMENT
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Set up stack pointer di bagian atas segmen kernel
    mov sp, 0xFFF0
    mov bp, sp

    ; Lompat ke awal kode kernel
    jmp KERNEL_SEGMENT:0x0000

failed_to_load:
    ; Loop tak terbatas jika gagal memuat kernel
    jmp $

; Padding untuk membuat bootloader pas 512 bytes
times 510-($-$$) db 0
dw 0xAA55
