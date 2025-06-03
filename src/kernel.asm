; src/kernel.asm
bits 32

SECTION .note.GNU-stack

; Hapus underscore dari deklarasi global
global putInMemory
global interrupt
global getBiosTick

section .text

; Hapus underscore dari definisi label
getBiosTick:
    push ebp
    mov ebp, esp
    mov ah, 0x00
    int 0x1A
    movzx eax, dx
    pop ebp
    ret

; Hapus underscore dari definisi label
putInMemory:
    push ebp
    mov ebp, esp
    push ds
    mov ax, [ebp+8]   
    mov esi, [ebp+12] 
    mov cl, [ebp+16]  
    mov ds, ax        
    mov [esi], cl     
    pop ds
    pop ebp
    ret

; Hapus underscore dari definisi label
interrupt:
    push ebp
    mov ebp, esp
    push edi
    push esi
    push ebx 
    push ecx
    push edx

    mov eax, [ebp+8]    ; int_number (parameter pertama)
    mov [actual_interrupt_byte], al ; Patch dengan AL

    mov eax, [ebp+12]   ; Parameter kedua (untuk reg_eax interrupt)
    mov ebx, [ebp+16]   ; Parameter ketiga (untuk reg_ebx interrupt)
    mov ecx, [ebp+20]   ; Parameter keempat (untuk reg_ecx interrupt)
    mov edx, [ebp+24]   ; Parameter kelima (untuk reg_edx interrupt)

interrupt_instruction_placeholder:
    db 0xCD                 
actual_interrupt_byte:
    db 0x00                 

    pop edx
    pop ecx
    pop ebx
    pop esi
    pop edi
    pop ebp
    ret
