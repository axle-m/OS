#include "ktypes.h"

// sets n bytes of memory starting at s to byte c
void *memset(void *s, int c, size_t n);

// compares n bytes of two memory regions
int memcmp(const void *s1, const void *s2, size_t n);

// copies n bytes from src to dest (no overlap allowed)
void *memcpy(void *dest, const void *src, size_t n);

// copies n bytes from src to dest (safe for overlapping regions)
void *memmove(void *dest, const void *src, size_t n);

// searches for byte c in first n bytes of memory block s
void *memchr(const void *s, int c, size_t n);