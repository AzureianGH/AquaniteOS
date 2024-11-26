#pragma once

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);
void outl(uint16_t port, uint32_t val);
uint32_t inl(uint16_t port);
void outsb(uint16_t port, const void* addr, uint32_t count);
void insb(uint16_t port, void* addr, uint32_t count);
void outsw(uint16_t port, const void* addr, uint32_t count);
void insw(uint16_t port, void* addr, uint32_t count);
void outsl(uint16_t port, const void* addr, uint32_t count);
void insl(uint16_t port, void* addr, uint32_t count);
void io_wait();
#ifdef __cplusplus
}
#endif