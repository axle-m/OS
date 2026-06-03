; Print a single character in AL
print_char:
    mov ah, 0x0E    ; Teletype function
    mov bh, 0x00    ; Display page 0
    mov bl, 0x07    ; White on black color
    int 0x10
    ret

; Print a null-terminated string (DS:SI -> string)
print_string:
.print_loop:
    lodsb           ; Load byte from [DS:SI] into AL, increment SI
    cmp al, 0       ; Check for null-terminator (0)
    je .done
    call print_char
    jmp .print_loop
.done:
    ret