[BITS 32]

global _start
extern kmain

_start:
    call kmain
    cli
.halt:
    hlt
    jmp .halt
