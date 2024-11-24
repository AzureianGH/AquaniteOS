#include <string/string.h>

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

