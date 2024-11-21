#include <memory/heap.h>

uint64_t heap_start = 0;

void set_heap_start(uint64_t start) {
    heap_start = start;
}

void* sbrk(size_t increment) {
    uint64_t old_heap_start = heap_start;
    heap_start += increment;
    return (void*)old_heap_start;
}