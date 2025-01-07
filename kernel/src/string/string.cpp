#include <string/string.h>
#include <memory/dlmalloc.h>
#include <memory/gccmemory.h>
int strcmp(const char* str1, const char* str2)
{
    while (*str1 != '\0' && *str2 != '\0')
    {
        if (*str1 != *str2)
        {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return 0;
}

char *strtok(char *str, const char *delim)
{
    static char *last = NULL;
    if (str != NULL)
    {
        last = str;
    }
    if (last == NULL)
    {
        return NULL;
    }
    char *start = last;
    while (*last != '\0')
    {
        const char *d = delim;
        while (*d != '\0')
        {
            if (*last == *d)
            {
                *last = '\0';
                last++;
                if (start == last)
                {
                    start++;
                    continue;
                }
                return start;
            }
            d++;
        }
        last++;
    }
    return start;
}

char* strdup(const char* str)
{
    size_t len = strlen(str) + 1;
    char* copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

int strncmp(const char* str1, const char* str2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (str1[i] != str2[i])
        {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

char* strncpy(char* dest, const char* src, size_t n)
{
    size_t i = 0;
    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

char* stpcpy(char* dest, const char* src)
{
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return dest;
}

char* strstr(const char* haystack, const char* needle)
{
    size_t needle_len = strlen(needle);
    while (*haystack != '\0')
    {
        if (strncmp(haystack, needle, needle_len) == 0)
        {
            return (char*)haystack;
        }
        haystack++;
    }
    return NULL;
}