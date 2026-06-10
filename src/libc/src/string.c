#include "../headers/std.h"
#include "../headers/string.h"

size_t strlen(const char *str)
{
    size_t ct = 0;

    while (*str++)
        ct++;

    return ct;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str1 == *str2)
    {
        str1++;
        str2++;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}

char *strcpy(char *dest, const char *src)
{
    char *ret = dest;

    while ((*dest++ = *src++))
        ;

    return ret;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    char *ret = dest;

    while (n && (*dest++ = *src++))
        n--;

    while (n--)
        *dest++ = '\0';

    return ret;
}
