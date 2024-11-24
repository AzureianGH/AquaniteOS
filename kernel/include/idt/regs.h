#pragma once
#include <stdint.h>
struct registers_t {
  volatile uint64_t r15;
  volatile uint64_t r14;
  volatile uint64_t r13;
  volatile uint64_t r12;
  volatile uint64_t r11;
  volatile uint64_t r10;
  volatile uint64_t r9;
  volatile uint64_t r8;
  volatile uint64_t rsi;
  volatile uint64_t rdi;
  volatile uint64_t rbp;
  volatile uint64_t rdx;
  volatile uint64_t rcx;
  volatile uint64_t rbx;
  volatile uint64_t rax;
  volatile uint64_t core;
  volatile uint64_t isrNumber;
  volatile uint64_t errorCode;
  volatile uint64_t rip;
  volatile uint64_t cs;
  volatile uint64_t rflags;
  volatile uint64_t rsp;
  volatile uint64_t ss;
} __attribute__((packed));