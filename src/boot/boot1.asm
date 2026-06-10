[BITS 16]
[ORG 0x0500]

KERNEL_START_SECTOR equ 6
KERNEL_SECTORS      equ 32

; Temporary load location (< 1 MiB)
TEMP_SEG equ 0x1000
TEMP_OFF equ 0x0000

KERNEL_PHYS equ 0x00100000

start:

    mov [BOOT_DRIVE], dl

    xor ax, ax
    mov ds, ax

    mov si, msg
    call print_string

    ; a20

    in al, 0x92
    or al, 2
    out 0x92, al

    ; enter unreal mode

    cli

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:pmode

[BITS 32]

pmode:
        mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, cr0
    and eax, 0xFFFFFFFE
    mov cr0, eax

    jmp 0x0000:rmode

[BITS 16]

rmode:

    sti

; Read kernel into temporary buffer

    mov ax, TEMP_SEG
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0
    mov cl, KERNEL_START_SECTOR
    mov dh, 0
    mov dl, [BOOT_DRIVE]

    int 0x13
    jc disk_error

; Copy kernel to 1 MiB using unreal mode

    cld

    db 0x66
    mov si, 0          ; ESI = 0

    db 0x66
    mov di, 0x0000     ; EDI low word

    mov ax, TEMP_SEG
    mov ds, ax

    ; destination = 0x00100000
    ; ES still has 4 GiB limit cached

    db 0x66
    mov edi, KERNEL_PHYS

    db 0x66
    mov esi, 0x00010000

    mov cx, (KERNEL_SECTORS * 512) / 4

    a32 rep movsd

; Jump to kernel

    mov si, success_msg
    call print_string
    jmp 0xFFFF:0x0010

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

BOOT_DRIVE db 0

msg db "Loading kernel...",0
success_msg db "Kernel loaded",0
disk_error_msg db "Read error",0

; GDT

align 8

gdt:
    dq 0

    ; 32-bit code segment
    dq 0x00CF9A000000FFFF

    ; 32-bit data segment
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt

%include "print.asm"

times (2048 - ($ - $$)) db 0