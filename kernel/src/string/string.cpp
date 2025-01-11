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

int strcasecmp(const char* str1, const char* str2)
{
    while (*str1 != '\0' && *str2 != '\0')
    {
        char c1 = *str1;
        char c2 = *str2;
        if (c1 >= 'A' && c1 <= 'Z')
        {
            c1 += 32;
        }
        if (c2 >= 'A' && c2 <= 'Z')
        {
            c2 += 32;
        }
        if (c1 != c2)
        {
            return c1 - c2;
        }
        str1++;
        str2++;
    }
    return 0;
}

char* strrchr(const char* str, int c)
{
    const char* last = NULL;
    while (*str != '\0')
    {
        if (*str == c)
        {
            last = str;
        }
        str++;
    }
    return (char*)last;
}

int strncasecmp(const char* str1, const char* str2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        char c1 = str1[i];
        char c2 = str2[i];
        if (c1 >= 'A' && c1 <= 'Z')
        {
            c1 += 32;
        }
        if (c2 >= 'A' && c2 <= 'Z')
        {
            c2 += 32;
        }
        if (c1 != c2)
        {
            return c1 - c2;
        }
    }
    return 0;
}

int atoi(const char* str)
{
    int result = 0;
    while (*str != '\0')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

int isspace(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 32;
    }
    return c;
}

int toupper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 32;
    }
    return c;
}

//strchr
char* strchr(const char* str, int c)
{
    while (*str != '\0')
    {
        if (*str == c)
        {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}
#include <stdarg.h>

//vsscanf
int vsscanf(const char* str, const char* format, va_list args)
{
    int result = 0;
    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            if (*format == 'd')
            {
                int* value = va_arg(args, int*);
                *value = atoi(str);
                result++;
            }
            else if (*format == 'x')
            {
                int* value = va_arg(args, int*);
                *value = 0;
                while (*str != '\0')
                {
                    if (*str >= '0' && *str <= '9')
                    {
                        *value = *value * 16 + (*str - '0');
                    }
                    else if (*str >= 'a' && *str <= 'f')
                    {
                        *value = *value * 16 + (*str - 'a' + 10);
                    }
                    else if (*str >= 'A' && *str <= 'F')
                    {
                        *value = *value * 16 + (*str - 'A' + 10);
                    }
                    else
                    {
                        break;
                    }
                    str++;
                }
                result++;
            }
            else if (*format == 's')
            {
                char* value = va_arg(args, char*);
                while (*str != '\0' && !isspace(*str))
                {
                    *value = *str;
                    value++;
                    str++;
                }
                *value = '\0';
                result++;
            }
            else if (*format == 'c')
            {
                char* value = va_arg(args, char*);
                *value = *str;
                result++;
            }
            format++;
        }
        else
        {
            if (*str != *format)
            {
                break;
            }
            str++;
            format++;
        }
    }
    return result;
}

int sscanf(const char* str, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vsscanf(str, format, args);
    va_end(args);
    return result;
}

//atof
double atof(const char* str)
{
    double result = 0;
    double decimal = 0.1;
    int sign = 1;
    if (*str == '-')
    {
        sign = -1;
        str++;
    }
    while (*str != '\0')
    {
        if (*str == '.')
        {
            str++;
            while (*str != '\0')
            {
                result += (*str - '0') * decimal;
                decimal /= 10;
                str++;
            }
            break;
        }
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}