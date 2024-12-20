#include <memory/heap.h>
#include <flanterm/flantermglobal.h>
uint64_t heap_start0 = 0;

void set_heap_start(uint64_t start) {
    heap_start0 = start;
    lprintf(logging_level::INFO, "Heap start set to 0x%n\n", heap_start0);
    lprintf(logging_level::OK, "Heap initialized.\n");
}

void* sbrk(size_t increment) {
    uint64_t old_heap_start = heap_start0;
    heap_start0 += increment;
    return (void*)old_heap_start;
}