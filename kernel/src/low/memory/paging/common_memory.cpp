#include <limine.h>
#include <memory/paging/common_memory.h>
#include <stdint.h>
#include <flanterm/flantermglobal.h>
limine_hhdm_response* hddm_response;
limine_memmap_response* lim_memory_map;
limine_kernel_address_response* lim_kernel_addy;

void set_up_common_mem(limine_hhdm_response* hhdm_response, limine_memmap_response* memory_map, limine_kernel_address_response* kernel_addy)
{
    hddm_response = hhdm_response;
    lim_memory_map = memory_map;
    lim_kernel_addy = kernel_addy;
    lprintf(logging_level::OK,"Common memory set up.\n");
}

uint64_t get_kernel_base_address()
{
    return lim_kernel_addy->virtual_base;
}

uint64_t get_highest_memory_map_address()
{
    auto memory_map = get_memory_map();
    uint64_t greatest_address = 0;
    for (uint64_t i = 0; memory_map->entry_count > i; i++)
    {
        auto entry = memory_map->entries[i];
        if (entry->base + entry->length > greatest_address)
            greatest_address = entry->base + entry->length;
    }
    return greatest_address;
}

uintptr_t get_higher_half_offset()
{
    // we're assuming that response isn't nullptr.
    return hddm_response->offset;
}

limine_memmap_response *get_memory_map()
{
    if (lim_memory_map == nullptr)
    {
        return nullptr;
    }

    return lim_memory_map;
}