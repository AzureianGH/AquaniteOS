
#include <memory/paging/vmm.h>
#include <memory/dlmalloc.h>
#include <flanterm/flantermglobal.h>
#include <memory/heap.h>
#include <stdint.h>
#include <stddef.h>
#include <memory/paging/physical.h>
constexpr uint64_t heap_size = 16 * 1024 * 1024 * 1024ULL;
uint64_t heap_start = 0; // Set an initial heap address
uint64_t heap_end = 0;
uint64_t allocated_memory = 0;
int errno = 0;

void expand_heap(size_t page_count)
{
    for (size_t i = 0; page_count > i; i++)
    {
        auto page_address = reinterpret_cast<uint64_t>(allocate_page());

        if (!page_address)
        {
            printf("Couldn't allocate page\n");
            asm volatile("cli; hlt");
        }

        bool resp = virtual_map(page_address, heap_end, true, false, true, false, false, false, 0, true);

        if (!resp)
        {
            printf("map failed.\n");
            asm volatile("cli;hlt");
        }

        heap_end += 4096;
    }
}

extern "C" int *__errno_location(void)
{
    return &errno;
}

extern "C" long sysconf(int name)
{
    switch (name)
    {
    case 30:
        return get_memory_statistics()->usable / 4096; // pages of memory
    default:
        break;
    }
    return 0;
}

extern "C" void *sbrk(long size)
{
    auto the_end = heap_end;
    long aligned_size = (size + 4095) & ~4095;

    expand_heap(aligned_size / 4096); // provide pages

    return reinterpret_cast<void *>(the_end);
}

extern "C" void *mmap(void *ptr, long size, long prot, long type, long handle, long arg)
{
    printf("Called mmap(ptr=%p, size=%ld, prot=%ld, type=%ld, handle=%ld, "
                 "arg=%ld)\n",
                 ptr, size, prot, type, handle, arg);
    asm volatile("cli; hlt");
    return nullptr;
}

extern "C" long munmap(void *ptr, long size)
{
    printf("Called munmap(ptr=%p, size=%ld)\n", ptr, size);
    asm volatile("cli; hlt");
    return 0; // Placeholder for munmap
}

void initialize_heap()
{
    heap_start = allocate_virtual_memory_kernel(heap_size);
    heap_end = heap_start;
}

void *operator new(size_t size)
{

    void *ptr = malloc(size); // Use malloc to allocate memory
    return ptr;
}

void *operator new[](size_t size)
{

    void *ptr = malloc(size); // Use malloc to allocate memory
    return ptr;
}

void operator delete(void *ptr, size_t size) noexcept
{

    if (ptr)
    {
        free(ptr);
    }
}

void operator delete(void *ptr) noexcept
{

    if (ptr)
    {
        free(ptr);
    }
}

void operator delete[](void *ptr, size_t size) noexcept
{

    if (ptr)
    {
        free(ptr);
    }
}

void operator delete[](void *ptr) noexcept
{

    if (ptr)
    {
        free(ptr);
    }
}