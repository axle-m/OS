#include <stdint.h>
#include "shell.h"
#include "lexer.h"
#include "parser.h"

extern uint8_t inb(uint16_t port);
extern void putchar(char c);
extern void print(const char *str);

extern int cursor_pos;
int prompt_limit = 0;

const char scancode_to_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '};


void launch_shell() {
    char input_buffer[256];
    int buffer_len = 0;
    

    uint8_t last_scancode = 0;

    for (;;)
    {
        if (inb(0x64) & 1)
        {
            uint8_t scancode = inb(0x60);

            if (scancode < 0x80 && scancode != last_scancode)
            {
                if (scancode < sizeof(scancode_to_ascii))
                {
                    char c = scancode_to_ascii[scancode];
                    if (c != 0)
                    {
                        putchar(c);
                    }
                    if(c == '\n'){
                        buffer_len = 0;
                        printe("> ");
                        prompt_limit = cursor_pos;
                    }

                }
            }

            last_scancode = scancode;
        }
        else
        {
            last_scancode = 0;
        }
    }
}