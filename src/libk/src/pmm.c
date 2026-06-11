#include "../include/pmm.h"

#define MAX_PAGES 262144

static uint32_t free_pages[MAX_PAGES];

static uint32_t free_page_count = 0;
static uint32_t total_page_count = 0;

static int ranges_overlap(
    uint64_t a_start,
    uint64_t a_end,
    uint64_t b_start,
    uint64_t b_end)
{
    return !(a_end <= b_start || b_end <= a_start);
}

void pmm_init(boot_info *boot)
{
    free_page_count = 0;
    total_page_count = 0;

    uint64_t kernel_start = boot->kernel_phys_start;
    uint64_t kernel_end = boot->kernel_phys_end;

    uint64_t heap_start = (uint32_t)boot->heap_start;
    uint64_t heap_end = heap_start + boot->heap_size;

    uint64_t boot_start = (uint32_t)boot;
    uint64_t boot_end = boot_start + sizeof(boot_info);

    uint64_t mmap_start = (uint32_t)boot->memory_map;
    uint64_t mmap_end =
        mmap_start +
        boot->memory_map_count * sizeof(e820_entry);

    for (uint32_t i = 0; i < boot->memory_map_count; i++)
    {
        e820_entry *entry = &boot->memory_map[i];

        if (entry->type != 1)
            continue;

        uint64_t region_start = entry->base;
        uint64_t region_end = entry->base + entry->length;

        if (region_end <= 0x100000)
            continue;

        if (region_start < 0x100000)
            region_start = 0x100000;

        region_start =
            (region_start + PAGE_SIZE - 1) &
            ~(PAGE_SIZE - 1);

        for (uint64_t addr = region_start;
             addr + PAGE_SIZE <= region_end;
             addr += PAGE_SIZE)
        {
            total_page_count++;

            uint64_t page_start = addr;
            uint64_t page_end = addr + PAGE_SIZE;

            if (ranges_overlap(
                    page_start,
                    page_end,
                    kernel_start,
                    kernel_end))
                continue;

            if (ranges_overlap(
                    page_start,
                    page_end,
                    heap_start,
                    heap_end))
                continue;

            if (ranges_overlap(
                    page_start,
                    page_end,
                    boot_start,
                    boot_end))
                continue;

            if (ranges_overlap(
                    page_start,
                    page_end,
                    mmap_start,
                    mmap_end))
                continue;

            if (free_page_count >= MAX_PAGES)
                return;

            free_pages[free_page_count++] =
                (uint32_t)addr;
        }
    }
}

void *pmm_alloc_page(void)
{
    if (free_page_count == 0)
        return 0;

    return (void *)free_pages[--free_page_count];
}

void pmm_free_page(void *page)
{
    if (!page)
        return;

    if (free_page_count >= MAX_PAGES)
        return;

    free_pages[free_page_count++] =
        (uint32_t)page;
}

uint32_t pmm_free_pages(void)
{
    return free_page_count;
}

uint32_t pmm_total_pages(void)
{
    return total_page_count;
}