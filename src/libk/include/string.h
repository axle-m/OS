#pragma once

#include <stddef.h> // for size_t

// returns the size of str in bytes
size_t strlen(const char *str);

// returns the difference of the first byte in which str1 and str2 differ
int strcmp(const char *str1, const char *str2);

// copies src into dest with no bounds check
char *strcpy(char *dest, const char *src);

// copies at most n characters of src into dest, pads with '\0' if n < strlen(src)
char *strncpy(char *dest, const char *src, size_t n);

// concatenates src onto dest, assumes dest has enough space
char *strcat(char *dest, const char *src);

// concatenates at most n characters from src onto dest, null-terminates result
char *strncat(char *dest, const char *src, size_t n);

// compares at most n bytes of two strings
int strncmp(const char *str1, const char *str2, size_t n);

// returns pointer to first occurrence of character c in str, or NULL if not found
char *strchr(const char *str, int c);

// returns pointer to last occurrence of character c in str, or NULL if not found
char *strrchr(const char *str, int c);

// returns pointer to first occurrence of substring needle in haystack, or NULL if not found
char *strstr(const char *haystack, const char *needle);

// returns length of initial segment of str not containing any chars from reject
size_t strcspn(const char *str, const char *reject);

// returns length of initial segment of str consisting only of chars from accept
size_t strspn(const char *str, const char *accept);

// returns pointer to first char in str that matches any char in accept, or NULL
char *strpbrk(const char *str, const char *accept);

// splits str into tokens using delimiters (modifies input string, stateful)
char *strtok(char *str, const char *delim);

// locale-aware string comparison (implementation-defined behavior)
int strcoll(const char *str1, const char *str2);