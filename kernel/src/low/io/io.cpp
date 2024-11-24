#include <io/io.h>

void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void outw(uint16_t port, uint16_t val) {
  asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void outl(uint16_t port, uint32_t val) {
  asm volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

uint32_t inl(uint16_t port) {
  uint32_t ret;
  asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void outsb(uint16_t port, const void* addr, uint32_t count) {
  asm volatile("cld; rep outsb" : "+S"(addr), "+c"(count) : "d"(port));
}

void insb(uint16_t port, void* addr, uint32_t count) {
  asm volatile("cld; rep insb" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}

void outsw(uint16_t port, const void* addr, uint32_t count) {
  asm volatile("cld; rep outsw" : "+S"(addr), "+c"(count) : "d"(port));
}

void insw(uint16_t port, void* addr, uint32_t count) {
  asm volatile("cld; rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}

void outsl(uint16_t port, const void* addr, uint32_t count) {
  asm volatile("cld; rep outsl" : "+S"(addr), "+c"(count) : "d"(port));
}

void insl(uint16_t port, void* addr, uint32_t count) {
  asm volatile("cld; rep insl" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}

void io_wait() {
  asm volatile("outb %%al, $0x80" : : "a"(0));
}