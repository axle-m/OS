#pragma once

#include "ktypes.h"

// sets n bytes of memory starting at s to byte c
void *kmemset(void *s, int c, size_t n);

// compares n bytes of two memory regions
int kmemcmp(const void *s1, const void *s2, size_t n);

// copies n bytes from src to dest (no overlap allowed)
void *mkemcpy(void *dest, const void *src, size_t n);

// copies n bytes from src to dest (safe for overlapping regions)
void *kmemmove(void *dest, const void *src, size_t n);

// searches for byte c in first n bytes of memory block s
void *kmemchr(const void *s, int c, size_t n);