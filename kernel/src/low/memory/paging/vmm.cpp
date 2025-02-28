#include <memory/paging/vmm.h>
#include <memory/paging/common_memory.h>
#include <memory/paging/physical.h>
#include <memory/dlmalloc.h>
#include <stdint.h>
#include <flanterm/flantermglobal.h>
#include <memory/gccmemory.h>
inline pml4 *get_pml4()
{
    uint64_t cr3 = 0;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));
    cr3 &= 0xfffffffffffff000;
    cr3 += get_higher_half_offset();
    return reinterpret_cast<pml4 *>(cr3);
}

uint64_t base_address = 0;
uint64_t remaining_bytes = 0;

void initialize_virtual_memory(void)
{
    base_address = get_higher_half_offset() + get_highest_memory_map_address();
    remaining_bytes = get_kernel_base_address() - base_address;
    lprintf(logging_level::INFO,"Bytes in the hole: 0x%u (B)\n", remaining_bytes);
    lprintf(logging_level::INFO,"Base address: 0x%l\n", base_address);
    lprintf(logging_level::OK,"Virtual memory initialized.\n");
}
bool virtual_map_page(void* page, uint64_t cr2) 
{
    return virtual_map(reinterpret_cast<uint64_t>(page), cr2, true, false, true, false, false, false, 0, true);
}

bool virtual_map(uint64_t physical_address, uint64_t virtual_address, bool is_read_write, bool disable_execution,
                 bool is_supervisor, bool pwt, bool pcd, bool pat, uint8_t pk, bool is_present)
{
    // Helper function: Check if an address is canonical
    auto is_canonical = [](uint64_t addr) -> bool {
        uint64_t high_bits = addr >> 48;
        return high_bits == 0 || high_bits == 0xFFFF;
    };

    // Alignment check for physical and virtual addresses
    if (physical_address % 4096 != 0)
    {
        lprintf(logging_level::ERROR,"Error: Physical address not aligned to 4 KiB.\n");
        asm volatile("cli; hlt");
        return false;
    }

    if (virtual_address % 4096 != 0)
    {
        asm volatile("cli; hlt");
        return false;
    }

    // Canonicality check for the virtual address
    if (!is_canonical(virtual_address))
    {
        return false;
    }

    // Decompose the virtual address into its indexes
    auto indexes = split_virtual_address_to_structure(virtual_address);

    // Flags setup
    bool map_present = is_present;

    // PML4 setup
    auto *pml4 = get_pml4();
    if (pml4[indexes.pml4].pdp_ppn == 0)
    {
        uint64_t p = reinterpret_cast<uint64_t>(allocate_page());
        if (!p)
        {
            return false;
        }
        memset(reinterpret_cast<void *>(p + get_higher_half_offset()), 0, 4096);
        pml4[indexes.pml4].pdp_ppn = p >> 12;
    }

    // Apply flags to PML4E
    auto &pml4e = pml4[indexes.pml4];
    pml4e.p = map_present;
    pml4e.rw = is_read_write;
    pml4e.nx = disable_execution;
    pml4e.us = is_supervisor;

    // PDP setup
    auto *pdp = reinterpret_cast<struct pdp *>((pml4e.pdp_ppn << 12) + get_higher_half_offset());
    if (pdp[indexes.pdp].pd_ppn == 0)
    {
        uint64_t p = reinterpret_cast<uint64_t>(allocate_page());
        if (!p)
        {
            lprintf(logging_level::ERROR,"Error: Failed to allocate PD.\n");
            return false;
        }
        memset(reinterpret_cast<void *>(p + get_higher_half_offset()), 0, 4096);
        pdp[indexes.pdp].pd_ppn = p >> 12;
    }

    // Apply flags to PDPE
    auto &pdpe = pdp[indexes.pdp];
    pdpe.p = map_present;
    pdpe.rw = is_read_write;
    pdpe.nx = disable_execution;
    pdpe.us = is_supervisor;

    // PD setup
    auto *pd = reinterpret_cast<struct pd *>((pdpe.pd_ppn << 12) + get_higher_half_offset());
    if (pd[indexes.pd].pt_ppn == 0)
    {
        uint64_t p = reinterpret_cast<uint64_t>(allocate_page());
        if (!p)
        {
            lprintf(logging_level::ERROR,"Error: Failed to allocate PT.\n");
            return false;
        }
        memset(reinterpret_cast<void *>(p + get_higher_half_offset()), 0, 4096);
        pd[indexes.pd].pt_ppn = p >> 12;
    }

    // Apply flags to PDE
    auto &pde = pd[indexes.pd];
    pde.p = map_present;
    pde.rw = is_read_write;
    pde.nx = disable_execution;
    pde.us = is_supervisor;

    // PT setup
    auto *pt = reinterpret_cast<struct pt *>((pde.pt_ppn << 12) + get_higher_half_offset());
    pt[indexes.pt].phys_ppn = physical_address >> 12;

    // Apply flags to PTE
    auto &pte = pt[indexes.pt];
    pte.p = map_present;
    pte.rw = is_read_write;
    pte.nx = disable_execution;
    pte.us = is_supervisor;
    pte.pwt = pwt;
    pte.pcd = pcd;
    pte.pat = pat;
    pte.pk = pk;

    // Invalidate TLB for the virtual address
    invlpg(virtual_address);

    return true;
}

char virtual_unmap(uint64_t virtual_address)
{
    auto indexes = split_virtual_address_to_structure(virtual_address);

    auto *pml4 = get_pml4();
    auto &pml4_entry = pml4[indexes.pml4];
    if (!pml4_entry.p)
    {
        return 0;
    }

    auto *pdp = reinterpret_cast<struct pdp *>((pml4_entry.pdp_ppn << 12) + get_higher_half_offset());
    auto &pdp_entry = pdp[indexes.pdp];
    if (!pdp_entry.p)
    {
        return 0;
    }

    auto *pd = reinterpret_cast<struct pd *>((pdp_entry.pd_ppn << 12) + get_higher_half_offset());
    auto &pd_entry = pd[indexes.pd];
    if (!pd_entry.p)
    {
        return 0;
    }

    auto *pt = reinterpret_cast<struct pt *>((pd_entry.pt_ppn << 12) + get_higher_half_offset());
    auto &pt_entry = pt[indexes.pt];
    if (!pt_entry.p)
    {
        return 0;
    }

    pt_entry.p = 0;
    invlpg(virtual_address);

    return 1;
}

void print_page_info(uint64_t virtual_address)
{
    if ((virtual_address & 0xffff000000000000) != 0xffff000000000000 && (virtual_address & 0xffff000000000000) != 0)
    {

        return;
    }

    auto pml4 = get_pml4();
    auto indexes = split_virtual_address_to_structure(virtual_address);

    // Check PML4 entry
    auto &pml4_entry = pml4[indexes.pml4];
    if (!pml4_entry.p)
    {
        lprintf(logging_level::ERROR,"PML4 entry not present for address: %016lX\n", virtual_address);
        return;
    }
    lprintf(logging_level::ERROR,"PML4[%u]: Present=%d RW=%d US=%d PWT=%d PCD=%d NX=%d PDP_PPN=%016lX\n", indexes.pml4, pml4_entry.p,
                 pml4_entry.rw, pml4_entry.us, pml4_entry.pwt, pml4_entry.pcd, pml4_entry.nx, pml4_entry.pdp_ppn);

    // Get PDP entry
    auto *pdp = reinterpret_cast<struct pdp *>((pml4_entry.pdp_ppn << 12) + get_higher_half_offset());
    auto &pdp_entry = pdp[indexes.pdp];
    if (!pdp_entry.p)
    {
        lprintf(logging_level::ERROR,"PDP entry not present for address: %016lX\n", virtual_address);
        return;
    }
    lprintf(logging_level::ERROR,"PDP[%u]: Present=%d RW=%d US=%d PWT=%d PCD=%d NX=%d PD_PPN=%016lX\n", indexes.pdp, pdp_entry.p,
                 pdp_entry.rw, pdp_entry.us, pdp_entry.pwt, pdp_entry.pcd, pdp_entry.nx, pdp_entry.pd_ppn);

    // Get PD entry
    auto *pd = reinterpret_cast<struct pd *>((pdp_entry.pd_ppn << 12) + get_higher_half_offset());
    auto &pd_entry = pd[indexes.pd];
    if (!pd_entry.p)
    {
        lprintf(logging_level::ERROR,"PD entry not present for address: %016lX\n", virtual_address);
        return;
    }
    lprintf(logging_level::ERROR,"PD[%u]: Present=%d RW=%d US=%d PWT=%d PCD=%d NX=%d PT_PPN=%016lX\n", indexes.pd, pd_entry.p,
                 pd_entry.rw, pd_entry.us, pd_entry.pwt, pd_entry.pcd, pd_entry.nx, pd_entry.pt_ppn);

    // Get PT entry
    auto *pt = reinterpret_cast<struct pt *>((pd_entry.pt_ppn << 12) + get_higher_half_offset());
    auto &pt_entry = pt[indexes.pt];
    if (!pt_entry.p)
    {
        lprintf(logging_level::ERROR,"PT entry not present for address: %016lX\n", virtual_address);
        return;
    }
    lprintf(logging_level::ERROR,"PT[%u]: Present=%d RW=%d US=%d PWT=%d PCD=%d PAT=%d NX=%d "
                 "Phys_PPN=%016lX\n",
                 indexes.pt, pt_entry.p, pt_entry.rw, pt_entry.us, pt_entry.pwt, pt_entry.pcd, pt_entry.pat,
                 pt_entry.nx, pt_entry.phys_ppn);

    // Calculate physical address
    uint64_t physical_address = (pt_entry.phys_ppn << 12);
    lprintf(logging_level::ERROR,"Mapped Physical Address: %016lX\n", physical_address);
}

uint64_t allocate_virtual_memory_kernel(uint64_t size)
{
    if (remaining_bytes - size <= 0)
    {
        asm volatile("cli; hlt");
    }

    auto address = base_address;
    base_address += size;
    return address | 0xffff000000000000;
}

void allocate_to(uint64_t virtual_base, uint64_t bytes)
{
    bytes = (bytes + 4096) & ~4095; // align
    size_t pages_needed = bytes / 4096;
    for (size_t i = 0; pages_needed > i; i++)
    {
        auto page_address = reinterpret_cast<uint64_t>(allocate_page());
        virtual_map(page_address, virtual_base + (i * 4096), true, false, true, false, false, false, 0, true);
    }
}