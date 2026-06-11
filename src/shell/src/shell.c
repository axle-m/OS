#include "../include/shell.h"

#define MAX_ARGS 16

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

void launch_shell()
{
    char input_buffer[256];
    char *ptr = input_buffer;

    uint8_t last_scancode = 0;

    print("> ");

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

                    if (c == '\n')
                    {
                        putchar(c);
                        *ptr = '\0';

                        // pass to lexer here
                        char **lexed = lex(input_buffer);

                         //print out the lexed array of words
                         // for(int i = 0; parsed[i] != NULL; i++){
                        //     print(parsed[i]);
                        //     print("\n");
                        // }

                        parsed_command_t cmd;
                        parse(lexed, &cmd);
                        
                        // execute_command(&cmd);


                        print("> ");
                        prompt_limit = cursor_pos;

                        ptr = input_buffer;
                    }
                    else if (c == '\b')
                    {
                        if (ptr > input_buffer)
                        {
                            putchar(c);
                            ptr--;
                            *ptr = '\0';
                        }
                    }
                    else if (c != 0)
                    {
                        if (ptr < input_buffer + 255)
                        {
                            putchar(c);
                            *ptr = c;
                            ptr++;
                        }
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