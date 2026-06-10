[BITS 16]
[ORG 0x0500]

KERNEL_START_SECTOR equ 6
KERNEL_SECTORS      equ 32

KERNEL_SEG equ 0x1000
KERNEL_OFF equ 0x0000

start:

    xor ax, ax
    mov ds, ax
    mov es, ax

    mov si, msg
    call print_string

    ; load kernel

    mov ax, KERNEL_SEG
    mov es, ax
    mov bx, KERNEL_OFF

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0
    mov cl, KERNEL_START_SECTOR
    mov dh, 0

    ; DL still contains boot drive from boot0
    int 0x13
    jc disk_error

    mov si, success_msg
    call print_string

    jmp KERNEL_SEG:KERNEL_OFF

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

msg db "Loading kernel...",0
success_msg db "Kernel loaded",0
disk_error_msg db "Kernel read error",0

%include "print.asm"

times (2048 - ($ - $$)) db 0