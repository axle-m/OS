#include <stdint.h>
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
int cursor_pos = 0;
int prompt_limit = 0;

const char scancode_to_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++){
        vga_buffer[i] = (0x0F << 8) | ' ';
    }
    cursor_pos = 0;
}

void putchar(char c){
    if(c == '\n'){
        cursor_pos = (cursor_pos / 80 + 1) * 80;
    } else if (c == '\b'){
        if(cursor_pos > prompt_limit){
            cursor_pos--;
            vga_buffer[cursor_pos] = (0x0F << 8) | ' ';
        }
    } else{
        vga_buffer[cursor_pos] = (0x0F << 8) | c;
        cursor_pos++;
    }
    if (cursor_pos >= 80*25) clear_screen();
}

void print(const char* str){
    for(int i = 0; str[i] != '\0'; i++) putchar(str[i]);
}

void kmain(void)
{
    clear_screen();
    print("Loaded Kernel\n");
    print("> ");
    prompt_limit = cursor_pos;
    
    uint8_t last_scancode = 0;
    
    while(1){
        if(inb(0x64) & 1){
            uint8_t scancode = inb(0x60);

            if(scancode < 0x80 && scancode != last_scancode) {
                if(scancode < sizeof(scancode_to_ascii)){
                    char c = scancode_to_ascii[scancode];
                    if(c != 0){
                        putchar(c);
                    }
                }
            }

            last_scancode = scancode;
        } else{
            last_scancode = 0;
        }
    }
}