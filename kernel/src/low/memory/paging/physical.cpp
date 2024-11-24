#include <memory/paging/physical.h>
#include <memory/paging/common_memory.h>
#include <stddef.h>
#include <stdint.h>
#include <flanterm/flantermglobal.h>
#include <sched/sched.h>
constexpr const char *type_to_string(uint64_t type)
{
    switch (type)
    {
    case LIMINE_MEMMAP_USABLE:
        return "Usable";
    case LIMINE_MEMMAP_RESERVED:
        return "Reserved";
    case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
        return "ACPI Reclaimable";
    case LIMINE_MEMMAP_ACPI_NVS:
        return "ACPI NVS";
    case LIMINE_MEMMAP_BAD_MEMORY:
        return "Bad Memory";
    case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
        return "Bootloader Reclaimable";
    case LIMINE_MEMMAP_KERNEL_AND_MODULES:
        return "Kernel and Modules";
    case LIMINE_MEMMAP_FRAMEBUFFER:
        return "Framebuffer";
    default:
        return "Unknown entry";
    }
}

// A structure to hold freelist for O(1) allocations.
struct freelist_entry
{
    freelist_entry *next;
    size_t size;
};

freelist_entry *head = nullptr;

static void push_to_freelist(freelist_entry *entry)
{
    if (head == nullptr)
    {
        head = entry;
        return;
    }

    entry->next = head;
    head = entry;
}

static freelist_entry *pop_from_freelist()
{
    if (head == nullptr)
    {
        return nullptr;
    }

    freelist_entry *entry = head;
    head = head->next;
    return entry;
}

memory_statistics mem_stats;

memory_statistics *get_memory_statistics()
{
    return &mem_stats;
}
spinlock_t allocate_spinlock;
void initialize_phys_memory(void)
{
    spinlock_init(&allocate_spinlock);
    auto memory_map = get_memory_map();
    if (memory_map == nullptr)
    {
        printf("No memory map has been found.\n");
        asm volatile("cli; hlt");
    }

    // Tag usable memory using hhdm.
    for (uint64_t i = 0; memory_map->entry_count > i; i++)
    {
        auto entry = memory_map->entries[i];

        mem_stats.overall += entry->length;
        switch (entry->type)
        {
        case LIMINE_MEMMAP_USABLE:
            mem_stats.usable += entry->length;
            break;
        case LIMINE_MEMMAP_RESERVED:
            mem_stats.reserved += entry->length;
            break;
        case LIMINE_MEMMAP_BAD_MEMORY:
            mem_stats.bad += entry->length;
            break;
        case LIMINE_MEMMAP_KERNEL_AND_MODULES:
            mem_stats.kernel += entry->length;
            break;
        case LIMINE_MEMMAP_FRAMEBUFFER:
            mem_stats.other += entry->length;
            break;
        case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
            mem_stats.reclaimable += entry->length;
            break;
        case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
            mem_stats.reclaimable += entry->length;
            break;
        default:
            break;
        }

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            freelist_entry *freelist_entry = reinterpret_cast<struct freelist_entry *>(
                reinterpret_cast<uintptr_t>(entry->base) + get_higher_half_offset());
            freelist_entry->next = nullptr;
            freelist_entry->size = entry->length;
            push_to_freelist(freelist_entry);
        }
    }
}


void *allocate_page(void)
{
    spinlock_acquire(&allocate_spinlock);
    freelist_entry *entry = pop_from_freelist();
    if (entry == nullptr)
    {
        printf("No available pages to allocate.\n");
        spinlock_release(&allocate_spinlock);
        return nullptr;
    }

    if (entry->size > 4096)
    {
        freelist_entry *new_entry = reinterpret_cast<freelist_entry *>(reinterpret_cast<uintptr_t>(entry) + 4096);
        new_entry->size = entry->size - 4096;
        new_entry->next = entry->next;
        entry->next = nullptr;
        entry->size = 4096;

        push_to_freelist(new_entry);
    }

    uintptr_t base_address = reinterpret_cast<uintptr_t>(entry);
    mem_stats.usable -= 4096;
    spinlock_release(&allocate_spinlock);
    return reinterpret_cast<void *>(base_address - get_higher_half_offset());
}

void deallocate_page(void *page)
{
    spinlock_acquire(&allocate_spinlock);
    uintptr_t page_address = reinterpret_cast<uintptr_t>(page);
    freelist_entry *entry = reinterpret_cast<freelist_entry *>(page_address + get_higher_half_offset());
    mem_stats.usable += 4096;

    push_to_freelist(entry);
    spinlock_release(&allocate_spinlock);
}