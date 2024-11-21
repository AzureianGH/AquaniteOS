#pragma once
#include "flanterm.h"
#include "backends/fb.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void flanterm_set_instance(flanterm_context* inst);
void printf(const char *fmt, ...);
void vprintf(const char *fmt, va_list args);