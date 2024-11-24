#include "flantermglobal.h"
#include <stdarg.h>
#include <stddef.h>
#include <idt/idt.h>

flanterm_context* instance = nullptr;

void flanterm_set_instance(flanterm_context *inst)
{
    instance = inst;
}
int strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// itoa
void itoa(int value, char *str, int base)
{
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return;
    }

    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    if (tmp_value < 0)
    {
        *ptr++ = '-';
    }
    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void ultoa(unsigned long value, char *str, int base)
{
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return;
    }

    char *ptr = str, *ptr1 = str, tmp_char;
    unsigned long tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void uitoa(unsigned int value, char *str, int base)
{
    if (base < 2 || base > 36)
    {
        *str = '\0';
        return;
    }

    char *ptr = str, *ptr1 = str, tmp_char;
    unsigned int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

//vsnprintf
void vsnprintf(char *str, size_t size, const char *format, va_list args)
{
    char *buffer = str;
    const char *end = str + size;
    const char *fmt = format;
    while (*fmt != '\0')
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 's':
            {
                const char *s = va_arg(args, const char *);
                while (*s != '\0' && buffer < end)
                {
                    *buffer = *s;
                    buffer++;
                    s++;
                }
                break;
            }
            case 'c':
            {
                char c = va_arg(args, int);
                *buffer = c;
                buffer++;
                break;
            }
            case 'd':
            {
                int i = va_arg(args, int);
                char buf[32];
                itoa(i, buf, 10);
                const char *s = buf;
                while (*s != '\0' && buffer < end)
                {
                    *buffer = *s;
                    buffer++;
                    s++;
                }
                break;
            }
            case 'x':
            {
                int i = va_arg(args, int);
                char buf[32];
                itoa(i, buf, 16);
                const char *s = buf;
                while (*s != '\0' && buffer < end)
                {
                    *buffer = *s;
                    buffer++;
                    s++;
                }
                break;
            }
            case 'p': // can return negative values
            {
                void *p = va_arg(args, void *);
                char buf[32];
                itoa(*(int*)p, buf, 16);
                const char *s = buf;
                while (*s != '\0' && buffer < end)
                {
                    *buffer = *s;
                    buffer++;
                    s++;
                }
                break;
            }
            case 'n': // cannot, but uses hex
            {
                void *p = va_arg(args, void *);
                char buf[32];
                uitoa(*(unsigned int*)p, buf, 16);
                const char *s = buf;
                while (*s != '\0' && buffer < end)
                {
                    *buffer = *s;
                    buffer++;
                    s++;
                }
                break;
            }
            //u is like x but unsigned
            case 'u':
            {
                unsigned int i = va_arg(args, unsigned int);
                char buf[32];
                uitoa(i, buf, 10);
                const char *s = buf;
                while (*s != '\0' && buffer < end)
                {
                    *buffer = *s;
                    buffer++;
                    s++;
                }
                break;
            }
            case 'l':
            {
                unsigned long i = va_arg(args, unsigned long); // Get the unsigned long argument
                char buf[32]; // Buffer to hold the converted value
                ultoa(i, buf, 16); // Convert the unsigned long to a string in hexadecimal (base 16)
                
                const char *s = buf; // Pointer to the start of the string
                while (*s != '\0' && buffer < end) // Loop until the string ends or buffer is full
                {
                    *buffer = *s; // Copy the character to the buffer
                    buffer++; // Move to the next position in the buffer
                    s++; // Move to the next character in the string
                }
                break;
            }
            }
        }
        else
        {
            *buffer = *fmt;
            buffer++;
        }
        fmt++;
    }
    *buffer = '\0';
}
void printf(const char *fmt, uint32_t fg, uint32_t bg, ...)
{
    if (instance == nullptr)
    {
        return;
    }

    va_list args;
    va_start(args, bg);

    char buffer[1024];
    vsnprintf(buffer, 1024, fmt, args);
    size_t old_bg = instance->current_bg;
    size_t old_fg = instance->current_primary;
    instance->set_text_bg_rgb(instance, bg);
    instance->set_text_fg_rgb(instance, fg);
    flanterm_write(instance, buffer, strlen(buffer));
    instance->set_text_bg(instance, old_bg);
    instance->set_text_fg(instance, old_fg);

    va_end(args);
}
void printf(const char *fmt, ...)
{
    if (instance == nullptr)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    vsnprintf(buffer, 1024, fmt, args);

    flanterm_write(instance, buffer, strlen(buffer));

    va_end(args);
}
void lprintf(logging_level lvl, const char *fmt, ...)
{
    if (instance == nullptr)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    vsnprintf(buffer, 1024, fmt, args);

    switch (lvl)
    {
    case logging_level::OK:
        printf("[ \e[0;32mOKAY\033[0m ] %s", buffer);
        break;
    case logging_level::INFO:
        printf("[ \e[0;37mINFO\033[0m ] %s", buffer);
        break;
    case logging_level::WARN:
        printf("[ \e[0;33mWARN\033[0m ] %s", buffer);
        break;
    case logging_level::ERROR:
        printf("[ \e[1;31mFAIL\033[0m ] %s", buffer);
        break;
    }

    va_end(args);
}
void vprintf(const char *fmt, va_list args)
{
    if (instance == nullptr)
    {
        return;
    }

    char buffer[1024];
    vsnprintf(buffer, 1024, fmt, args);

    flanterm_write(instance, buffer, strlen(buffer));
}