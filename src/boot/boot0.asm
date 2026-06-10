[BITS 16]
[ORG 0x7C00]

BOOT1_LOAD_ADDR equ 0x0500
BOOT1_SECTORS   equ 4

start:

    xor ax, ax
    mov ds, ax
    mov es, ax

    mov [BOOT_DRIVE], dl

    mov si, msg
    call print_string

; boot1 is 4 sectors starting at sector 2

    mov ax, 0x0000
    mov es, ax
    mov bx, 0x0500

    mov ah, 0x02
    mov al, 4
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [BOOT_DRIVE]

    int 0x13
    jc disk_error

    mov si, success_msg
    call print_string

    jmp 0x0000:0x0500

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

BOOT_DRIVE db 0

msg db "Loading boot1...",0
disk_error_msg db "Disk Read Error",0
success_msg db "Read OK",0

%include "print.asm"

times 510-($-$$) db 0
dw 0xAA55