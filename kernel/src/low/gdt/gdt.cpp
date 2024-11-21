#include <gdt/gdt.h>

extern "C" void GDT_flush(uint64_t);

// gdt and tss
struct GDT_entry GDT[5];
struct GDT_ptr GDT_ptr;
struct TSS tss;

void GDT_set_gate(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran)
{
    GDT[num].base_low = (base & 0xFFFF);
    GDT[num].base_middle = (base >> 16) & 0xFF;
    GDT[num].base_high = (base >> 24) & 0xFF;

    GDT[num].limit_low = (limit & 0xFFFF);
    GDT[num].granularity = (limit >> 16) & 0x0F;

    GDT[num].granularity |= gran & 0xF0;
    GDT[num].access = access;
}

void GDT_init()
{
    GDT_ptr.limit = (sizeof(struct GDT_entry) * 5) - 1;
    GDT_ptr.base = (uint64_t)&GDT;

    GDT_set_gate(0, 0, 0, 0, 0);                // Null segment
    GDT_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    GDT_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    GDT_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    GDT_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    GDT_flush((uint64_t)&GDT_ptr);
}

void GDT_init_TSS(uint64_t rsp0)
{
    tss.reserved = 0;
    tss.rsp[0] = rsp0;
    tss.rsp[1] = 0;
    tss.rsp[2] = 0;
    tss.reserved2 = 0;
    tss.ist[0] = 0;
    tss.ist[1] = 0;
    tss.ist[2] = 0;
    tss.ist[3] = 0;
    tss.ist[4] = 0;
    tss.ist[5] = 0;
    tss.ist[6] = 0;
    tss.reserved3 = 0;
    tss.reserved4 = 0;
    tss.iomap_base = sizeof(struct TSS);

    GDT_set_gate(5, (uint64_t)&tss, sizeof(struct TSS), 0xE9, 0x00);

    __asm__ __volatile__("movw $0x2B, %ax\n"
                         "ltr %ax");
}

void GDT_init_TSS_init()
{
    //init gdt and tss
    GDT_init();
    GDT_init_TSS(0x1000);
}