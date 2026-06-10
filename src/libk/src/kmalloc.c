#include "../include/kmalloc.h"

heap_t kheap;

void kmalloc_init(void *heap_start, size_t size)
{
    kheap.start = heap_start;
    kheap.end = (char *)heap_start + size;

    kheap.total_size = size;
    kheap.used_size = 0;

    kheap.first = (kmalloc_header *)heap_start;

    kheap.first->size = size - sizeof(kmalloc_header);
    kheap.first->free = 1;
    kheap.first->next = NULL;
    kheap.first->prev = NULL;
}

void *kmalloc(size_t size)
{
    size = align16(size);

    kmalloc_header *cur = kheap.first;

    while (cur)
    {
        if (cur->free && cur->size >= size)
        {
            size_t remaining = cur->size - size;

            if (remaining > sizeof(kmalloc_header))
            {
                kmalloc_header *new_block =
                    (kmalloc_header *)((char *)cur + sizeof(kmalloc_header) + size);

                new_block->size = remaining - sizeof(kmalloc_header);
                new_block->free = 1;
                new_block->next = cur->next;
                new_block->prev = cur;

                if (cur->next)
                    cur->next->prev = new_block;

                cur->next = new_block;
                cur->size = size;
            }

            cur->free = 0;
            kheap.used_size += size;

            return DATA(cur);
        }

        cur = cur->next;
    }

    return NULL;
}

void kfree(void *ptr)
{
    if (!ptr)
        return;

    kmalloc_header *hdr = HDR(ptr);

    hdr->free = 1;

    // merge with next
    if (hdr->next && hdr->next->free)
    {
        hdr->size += sizeof(kmalloc_header) + hdr->next->size;
        hdr->next = hdr->next->next;

        if (hdr->next)
            hdr->next->prev = hdr;
    }

    // merge with prev
    if (hdr->prev && hdr->prev->free)
    {
        hdr->prev->size += sizeof(kmalloc_header) + hdr->size;
        hdr->prev->next = hdr->next;

        if (hdr->next)
            hdr->next->prev = hdr->prev;
    }
}