#pragma once

#include <stdint.h>
#include <stddef.h>

void set_heap_start(uint64_t start);

void* sbrk(size_t increment);