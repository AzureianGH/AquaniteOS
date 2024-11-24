#pragma once

#include <stdint.h>
#include <stddef.h>

//paging for x64

#define PAGE_SIZE 4096
#define PML4_ENTRIES 512
#define PDPT_ENTRIES 512
#define PD_ENTRIES 512
#define PT_ENTRIES 512

// Define page table entries
#define PTE_PRESENT 0x1
#define PTE_RW 0x2
#define PTE_USER 0x4
#define PTE_PWT 0x8
#define PTE_PCD 0x10
#define PTE_ACCESSED 0x20
#define PTE_DIRTY 0x40
#define PTE_PAT 0x80
#define PTE_GLOBAL 0x100

void paging_init();
void paging_map(uint64_t virt, uint64_t phys, uint64_t flags);
void paging_unmap(uint64_t virt);
void paging_switch(uint64_t pml4);
void paging_set(uint64_t pml4);
uint64_t paging_get();
uint64_t paging_get_phys(uint64_t virt);
void paging_free(uint64_t virt);
void paging_free_phys(uint64_t phys);
