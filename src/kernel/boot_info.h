#pragma once
#include "../libk/include/ktypes.h"

#define BOOT_INFO_MAGIC 0xC0FFEE42

typedef struct e820_entry
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} e820_entry;

typedef struct boot_info
{
    uint32_t magic;

    // memory map (E820 from BIOS)
    e820_entry *memory_map;
    uint32_t memory_map_count;

    // kernel physical location info
    uint32_t kernel_phys_start;
    uint32_t kernel_phys_end;

    // heap region chosen by bootloader
    void *heap_start;
    uint32_t heap_size;

} boot_info;