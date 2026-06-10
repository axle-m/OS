#pragma once

#include "ktypes.h"

typedef struct kmalloc_header
{
    size_t size; // size of user allocation
    int free;    // 1 if free, 0 if used

    struct kmalloc_header *next;
    struct kmalloc_header *prev;
} kmalloc_header;

typedef struct heap
{
    void *start; // start of heap region
    void *end;   // end of heap region

    kmalloc_header *first; // first block
    size_t total_size;
    size_t used_size;
} heap_t;

#define ALIGNMENT 16

static inline size_t align16(size_t size)
{
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

// global heap
extern heap_t kheap;

#define HDR(ptr) ((kmalloc_header *)((char *)(ptr) - sizeof(kmalloc_header)))
#define DATA(hdr) ((void *)((char *)(hdr) + sizeof(kmalloc_header)))

void kmalloc_init(void *heap_start, size_t size);

void *kmalloc(size_t size);

void kfree(void *ptr);

void *krealloc(void *ptr, size_t new_size);