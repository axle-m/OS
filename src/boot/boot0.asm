[BITS 16]
[ORG 0x7C00]
start:
    ; 1. Initialize segment registers to 0
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    ; Save the boot drive number provided by BIOS in DL
    mov [BOOT_DRIVE], dl

    ; 2. Print initial message
    mov si, msg
    call print_string

    ; 3. Configure and call read_sector to load Sector 2 to 0x0500
    mov ax, 0x0000
    mov es, ax          ; Destination Segment ES=0x0000
    mov bx, 0x0500      ; Destination Offset BX=0x0500
    mov dl, [BOOT_DRIVE]; Read from the drive we booted from
    mov ch, 0x00        ; Cylinder 0
    mov cl, 0x02        ; Sector 2
    mov dh, 0x00        ; Head 0
    call read_sector

    ; 4. jump to new data
    jmp 0x0500

BOOT_DRIVE db 0
msg db "Reading sector 2...", 0
%include "print.asm"
%include "disk_read.asm"

; Boot sector padding and signature
times 510 - ($ - $$) db 0
dw 0xAA55

; Sector 2
sector2_start:
    