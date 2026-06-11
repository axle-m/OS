#include "../libk/include/ktypes.h"
#include "../libk/include/kmalloc.h"
#include "boot_info.h"
#include "../shell/shell.h"

volatile uint16_t *vga_buffer = (uint16_t *)0xB8000;
int cursor_pos = 0;

extern void launch_shell();
extern int prompt_limit;

uint8_t inb(uint16_t port)
{
    uint8_t result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void clear_screen()
{
    for (int i = 0; i < 80 * 25; i++)
    {
        vga_buffer[i] = (0x0F << 8) | ' ';
    }
    cursor_pos = 0;
}

void putchar(char c)
{
    if (c == '\n')
    {
        cursor_pos = (cursor_pos / 80 + 1) * 80;
    }
    else if (c == '\b')
    {
        if (cursor_pos > prompt_limit)
        {
            cursor_pos--;
            vga_buffer[cursor_pos] = (0x0F << 8) | ' ';
        }
    }
    else
    {
        vga_buffer[cursor_pos] = (0x0F << 8) | c;
        cursor_pos++;
    }
    if (cursor_pos >= 80 * 25)
        clear_screen();
}

void print(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        putchar(str[i]);
}

void kmain(boot_info *boot)
{
    // set up memory
    if (boot->magic != BOOT_INFO_MAGIC)
    {
        while (1)
            ;
    }

    e820_entry *map = boot->memory_map;

    kmalloc_init(
        boot->heap_start,
        boot->heap_size);

    clear_screen();
    print("Loaded Kernel\n");

    launch_shell();
}