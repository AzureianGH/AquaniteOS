#pragma once
#include "flanterm.h"
#include "backends/fb.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
enum class logging_level
{
    OK,
    INFO,
    WARN,
    ERROR,
    DEBUG
};
void flanterm_set_instance(flanterm_context* inst);
void lprintf(logging_level lvl, const char *fmt, ...);
void vprintf(const char *fmt, va_list args);
#endif
#ifdef __cplusplus
extern "C"
{
#endif
    void clprintf(unsigned char lvl, const char *fmt, ...);
    void printf(const char *fmt, ...);
    int16_t asprintf(char **str, const char *fmt, ...);
    int16_t snprintf(char *str, size_t size, const char *fmt, ...);
    void vsnprintf(char *str, size_t size, const char *format, va_list args);
    void clear_screen();
    void set_cursor(size_t x, size_t y);
#ifdef __cplusplus
}
#endif