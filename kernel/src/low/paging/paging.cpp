#include <paging/paging.h>
#include <memory/dlmalloc.h>
#include <stdint.h>



// Page tables structures
uint64_t *page_dir_ptr_tab; // 4th-level page table (PML4)
uint64_t *page_dir;         // 3rd-level page table (PDPT)
uint64_t *page_table;       // 2nd-level page table (PD)
uint64_t *page_table_entries; // 1st-level entries (PT)

void paging_init()
{
    // Initialize the page directory pointer table (PML4)
    page_dir_ptr_tab = (uint64_t*)malloc(PAGE_SIZE);
    for (int i = 0; i < PML4_ENTRIES; i++)
    {
        page_dir_ptr_tab[i] = 0; // Set all entries to 0
    }
    
    // Create page directory pointer table entry
    page_dir = (uint64_t*)malloc(PAGE_SIZE);
    for (int i = 0; i < PDPT_ENTRIES; i++)
    {
        page_dir[i] = 0; // Set all entries to 0
    }
    
    // Create page table
    page_table = (uint64_t*)malloc(PAGE_SIZE);
    for (int i = 0; i < PD_ENTRIES; i++)
    {
        page_table[i] = 0; // Set all entries to 0
    }
    
    // Map the first page in memory
    paging_map(0x1000, 0x1000, PTE_PRESENT | PTE_RW | PTE_USER);
    
    // Set the PML4 entry to point to the page directory
    page_dir_ptr_tab[0] = (uint64_t)page_dir | PTE_PRESENT | PTE_RW;
}

void paging_map(uint64_t virt, uint64_t phys, uint64_t flags)
{
    // Map virtual address `virt` to physical address `phys` with the given flags
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index = (virt >> 21) & 0x1FF;
    uint64_t pt_index = (virt >> 12) & 0x1FF;
    
    // Check if the PML4 entry exists, otherwise allocate and set it
    if (page_dir_ptr_tab[pml4_index] == 0)
    {
        page_dir_ptr_tab[pml4_index] = (uint64_t)malloc(PAGE_SIZE) | PTE_PRESENT | PTE_RW;
    }
    
    // Check if the PDPT entry exists, otherwise allocate and set it
    uint64_t *pdpt = (uint64_t*) (page_dir_ptr_tab[pml4_index] & ~0xFFF);
    if (pdpt[pdpt_index] == 0)
    {
        pdpt[pdpt_index] = (uint64_t)malloc(PAGE_SIZE) | PTE_PRESENT | PTE_RW;
    }
    
    // Check if the PD entry exists, otherwise allocate and set it
    uint64_t *pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
    if (pd[pd_index] == 0)
    {
        pd[pd_index] = (uint64_t)malloc(PAGE_SIZE) | PTE_PRESENT | PTE_RW;
    }
    
    // Check if the PT entry exists, otherwise allocate and set it
    uint64_t *pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
    pt[pt_index] = (phys & ~0xFFF) | flags;
}

void paging_unmap(uint64_t virt)
{
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index = (virt >> 21) & 0x1FF;
    uint64_t pt_index = (virt >> 12) & 0x1FF;
    
    uint64_t *pdpt = (uint64_t*)(page_dir_ptr_tab[pml4_index] & ~0xFFF);
    uint64_t *pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
    uint64_t *pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
    
    pt[pt_index] = 0; // Clear the page table entry
}

void paging_switch(uint64_t pml4)
{
    // Switch to a new PML4 table (page directory)
    uint64_t cr3 = pml4; // Load new page directory address into CR3 register
    asm volatile("mov %0, %%cr3" : : "r"(cr3));
}

void paging_set(uint64_t pml4)
{
    // Same as paging_switch, but might be used for other purposes
    paging_switch(pml4);
}

uint64_t paging_get()
{
    uint64_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3)); // Get the value of the CR3 register (PML4 base)
    return cr3;
}

uint64_t paging_get_phys(uint64_t virt)
{
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index = (virt >> 21) & 0x1FF;
    uint64_t pt_index = (virt >> 12) & 0x1FF;
    
    uint64_t *pdpt = (uint64_t*)(page_dir_ptr_tab[pml4_index] & ~0xFFF);
    uint64_t *pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
    uint64_t *pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
    
    return pt[pt_index] & ~0xFFF; // Return physical address (clear flags)
}

void paging_free(uint64_t virt)
{
    uint64_t pml4_index = (virt >> 39) & 0x1FF;
    uint64_t pdpt_index = (virt >> 30) & 0x1FF;
    uint64_t pd_index = (virt >> 21) & 0x1FF;
    uint64_t pt_index = (virt >> 12) & 0x1FF;
    
    uint64_t *pdpt = (uint64_t*)(page_dir_ptr_tab[pml4_index] & ~0xFFF);
    uint64_t *pd = (uint64_t*)(pdpt[pdpt_index] & ~0xFFF);
    uint64_t *pt = (uint64_t*)(pd[pd_index] & ~0xFFF);
    
    pt[pt_index] = 0; // Clear the page table entry (free the page)
}

void paging_free_phys(uint64_t phys)
{
    // Physically free the page memory (requires the allocator to support free)
    free((void*)phys); 
}
