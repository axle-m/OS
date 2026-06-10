[BITS 16]
[ORG 0x0500]

KERNEL_START_SECTOR equ 6
KERNEL_SECTORS      equ 32

KERNEL_SEG equ 0xFFFF
KERNEL_OFF equ 0x0010      ; physical 0x100000

start: 
    

BOOT_DRIVE db 0

msg db "Loading kernel...",0

%include "print.asm"
%include "disk_read.asm"

; pad to exactly 4 sectors
times (2048 - ($ - $$)) db 0