#pragma once
// X64 GDT
#include <stdint.h>
#include <stddef.h>

struct GDT_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct GDT_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct TSS {
    uint32_t reserved;
    uint64_t rsp[3];
    uint64_t reserved2;
    uint64_t ist[7];
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iomap_base;
} __attribute__((packed));



void GDT_set_gate(int num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);
void GDT_init();
void GDT_init_TSS(uint64_t rsp0);

void GDT_init_TSS_init();