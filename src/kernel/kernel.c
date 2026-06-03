void kmain(void)
{
    volatile char *vga = (volatile char *)0xB8000;

    const char *msg = "Loaded kernel";

    while (*msg)
    {
        *vga++ = *msg++;
        *vga++ = 0x0F;
    }

    for (;;)
        __asm__ volatile("hlt");
}