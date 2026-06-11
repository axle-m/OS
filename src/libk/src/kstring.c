#include "../include/kstring.h"
#include "../include/ktypes.h"

size_t kstrlen(const char *str)
{
    size_t ct = 0;

    while (*str++)
        ct++;

    return ct;
}

int kstrcmp(const char *str1, const char *str2)
{
    while (*str1 && *str1 == *str2)
    {
        str1++;
        str2++;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}

char *kstrcpy(char *dest, const char *src)
{
    char *ret = dest;

    while ((*dest++ = *src++))
        ;

    return ret;
}

char *kstrncpy(char *dest, const char *src, size_t n)
{
    char *ret = dest;

    while (n && (*dest++ = *src++))
        n--;

    while (n--)
        *dest++ = '\0';

    return ret;
}

char *kstrcat(char *dest, const char *src)
{
    char *ret = dest;

    while (*dest)
        dest++;
    while ((*dest++ = *src++))
        ;

    return ret;
}

char *kstrncat(char *dest, const char *src, size_t n)
{
    char *ret = dest;

    while (*dest)
        dest++;

    while (n-- && *src)
        *dest++ = *src++;

    *dest = '\0';

    return ret;
}

char *kstrchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }
    return (*s == (char)c) ? (char *)s : NULL;
}

char *kstrrchr(const char *s, int c)
{
    const char *last = NULL;

    while (*s)
    {
        if (*s == (char)c)
            last = s;
        s++;
    }

    if ((char)c == *s)
        last = s;

    return (char *)last;
}

char *kstrstr(const char *haystack, const char *needle)
{
    if (!*needle)
        return (char *)haystack;

    for (; *haystack; haystack++)
    {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && (*h == *n))
        {
            h++;
            n++;
        }

        if (!*n)
            return (char *)haystack;
    }

    return NULL;
}

size_t kstrcspn(const char *s, const char *reject)
{
    size_t i = 0;

    for (; *s; s++, i++)
    {
        const char *r = reject;

        while (*r)
        {
            if (*s == *r)
                return i;
            r++;
        }
    }

    return i;
}

size_t kstrspn(const char *s, const char *accept)
{
    size_t i = 0;

    for (; *s; s++, i++)
    {
        const char *a = accept;
        int ok = 0;

        while (*a)
        {
            if (*s == *a)
            {
                ok = 1;
                break;
            }
            a++;
        }

        if (!ok)
            break;
    }

    return i;
}

char *kstrpbrk(const char *s, const char *accept)
{
    for (; *s; s++)
    {
        const char *a = accept;

        while (*a)
        {
            if (*s == *a)
                return (char *)s;
            a++;
        }
    }

    return NULL;
}

char *kstrtok(char *str, const char *delim)
{
    static char *next;

    if (str)
        next = str;

    if (!next)
        return NULL;

    char *token_start = next;

    // skip leading delimiters
    while (*token_start)
    {
        const char *d = delim;
        int is_delim = 0;

        while (*d)
        {
            if (*token_start == *d)
            {
                is_delim = 1;
                break;
            }
            d++;
        }

        if (!is_delim)
            break;

        token_start++;
    }

    if (!*token_start)
    {
        next = NULL;
        return NULL;
    }

    char *end = token_start;

    while (*end)
    {
        const char *d = delim;
        while (*d)
        {
            if (*end == *d)
            {
                *end = '\0';
                next = end + 1;
                return token_start;
            }
            d++;
        }
        end++;
    }

    next = NULL;
    return token_start;
}

int kstrcoll(const char *s1, const char *s2)
{
    return kstrcmp(s1, s2);
}