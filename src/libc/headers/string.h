#ifndef STRING_H
#define STRING_H

#include <stddef.h> // for size_t

// returns the size of str in bytes
size_t strlen(const char *str);

// returns the difference of the first byte in which str1 and str2 differ
int strcmp(const char *str1, const char *str2);

// copies src into dest with no bounds check
char *strcpy(char *dest, const char *src);

// copies at most n characters of src into dest, pads with '\0' if n < strlen(src)
char *strncpy(char *dest, const char *src, size_t n);

#endif
