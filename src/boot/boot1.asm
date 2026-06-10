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
    mov es, ax

    mov si, msg
    call print_string

    ; Enable A20 Line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Enter Unreal Mode
    cli

    lgdt [gdt_descriptor]

    mov eax, cr0
    or al, 1
    mov cr0, eax        ; Enable protected mode (16-bit segment rules still apply)

    ; Load data segment descriptors with 32-bit data selector (0x10) to cache 4GB limit
    mov bx, 0x10
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx

    ; Switch back to real mode immediately
    mov eax, cr0
    and al, 0xFE
    mov cr0, eax

    ; Far jump to flush instruction prefetch queue and reload CS
    jmp 0x0000:.unreal_done

.unreal_done:
    ; Restore DS and ES to real-mode segment (0x0000)
    ; Segment limit of 4GB remains in cached descriptors
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    sti

; Read kernel into temporary buffer (under 1 MiB)

    mov ax, TEMP_SEG
    mov es, ax
    xor bx, bx          ; Destination ES:BX = 0x1000:0x0000

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

    xor ax, ax
    mov ds, ax
    mov es, ax          ; Use flat 0-based segments for unreal copying

    ; Source: 0x00010000 (where TEMP_SEG:0 is)
    db 0x66
    mov esi, 0x00010000

    ; Destination: 0x00100000 (1 MiB)
    db 0x66
    mov edi, KERNEL_PHYS

    mov cx, (KERNEL_SECTORS * 512) / 4

    a32 rep movsd

; Jump to kernel: transition to 32-bit protected mode first

    mov si, success_msg
    call print_string

    cli

    ; Enable protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ; Set up segment selectors and stack pointer for 32-bit protected mode
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x00090000 ; safe stack below 640KB

    ; 32-bit far jump directly to 0x08:0x00100000
    db 0x66, 0xEA
    dd 0x00100000
    dw 0x0008

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