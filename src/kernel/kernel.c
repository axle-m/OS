#include "../libk/include/ktypes.h"
#include "../libk/include/kmalloc.h"
#include "../libk/include/kstring.h"
#include "boot_info.h"
#include "../shell/include/shell.h"
#include "../libk/include/pmm.h"

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
static inline void outw(uint16_t port, uint16_t value)
{
    __asm__ __volatile__("outw %0, %1"
                         :
                         : "a"(value), "Nd"(port));
}

void shutdown()
{
    // QEMU (isa-debug-exit / ACPI poweroff)
    outw(0x604, 0x2000);

    // Bochs / older QEMU
    outw(0xB004, 0x2000);

    // VirtualBox
    outw(0x4004, 0x3400);

    // halt forever in any other case
    while (1)
    {
        __asm__ __volatile__("cli");
        __asm__ __volatile__("hlt");
    }
}

void print_dec(uint32_t value)
{
    char buf[11];
    print(itoa(value, buf));
}

void kmain(boot_info *boot)
{
    if (boot->magic != BOOT_INFO_MAGIC)
    {
        print("Invalid boot info\n");
        while (1)
            ;
    }

    kmalloc_init(
        boot->heap_start,
        boot->heap_size);

    pmm_init(boot);

    clear_screen();

    print("Kernel loaded\n");

    print("Total pages: ");
    print_dec(pmm_total_pages());

    print("\nFree pages: ");
    print_dec(pmm_free_pages());

    print("\n");

    launch_shell();
}