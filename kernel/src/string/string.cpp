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
