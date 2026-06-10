[BITS 16]
[ORG 0x0500]

KERNEL_START_SECTOR equ 6
KERNEL_SECTORS      equ 32

; Temporary load location (< 1 MiB)
TEMP_SEG equ 0x1000
TEMP_OFF equ 0x0000

KERNEL_PHYS equ 0x00100000

BOOTINFO_ADDR equ 0x00080000
E820_ADDR     equ 0x00081000

BOOTINFO_MAGIC equ 0xC0FFEE42

BOOTINFO_MAGIC_OFF            equ 0
BOOTINFO_MEMORY_MAP_OFF       equ 4
BOOTINFO_MEMORY_MAP_COUNT_OFF equ 8
BOOTINFO_KERNEL_START_OFF     equ 12
BOOTINFO_KERNEL_END_OFF       equ 16
BOOTINFO_HEAP_START_OFF       equ 20
BOOTINFO_HEAP_SIZE_OFF        equ 24

BOOTINFO_PTR dd 0

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


; load memory and boot info
    mov ax, 0x8100      ; ES = 0x8100
    mov es, ax          ; ES:DI = 0x8100:0000 = 0x81000
    xor di, di

    xor ebx, ebx        ; continuation value
    xor bp, bp          ; entry count

.e820_loop:
    mov eax, 0xE820
    mov edx, 0x534D4150
    mov ecx, 24
    int 0x15
    jc .e820_done

    cmp eax, 0x534D4150
    jne .e820_done

    add di, 24
    inc bp

    test ebx, ebx
    jnz .e820_loop

.e820_done:

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

; load boot_info struct into memory
    mov edi, BOOTINFO_ADDR

    ; magic
    mov dword [edi + BOOTINFO_MAGIC_OFF], BOOTINFO_MAGIC

    ; memory map pointer
    mov dword [edi + BOOTINFO_MEMORY_MAP_OFF], E820_ADDR

    ; memory map count
    movzx eax, bp
    mov dword [edi + BOOTINFO_MEMORY_MAP_COUNT_OFF], eax

    ; kernel physical location
    mov dword [edi + BOOTINFO_KERNEL_START_OFF], KERNEL_PHYS

    mov eax, KERNEL_PHYS
    add eax, KERNEL_SECTORS * 512
    mov dword [edi + BOOTINFO_KERNEL_END_OFF], eax

    ; heap chosen by bootloader
    mov dword [edi + BOOTINFO_HEAP_START_OFF], 0x00200000

    ; 16 MiB heap
    mov dword [edi + BOOTINFO_HEAP_SIZE_OFF], 0x01000000

; Jump to kernel: transition to 32-bit protected mode first

    mov si, success_msg
    call print_string

    mov dword [BOOTINFO_PTR], BOOTINFO_ADDR

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

    mov eax, BOOTINFO_ADDR

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