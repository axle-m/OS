[BITS 32]

global _start
extern kmain

_start:
    push eax     ; boot_info *
    call kmain
    cli
.halt:
    hlt
    jmp .halt
