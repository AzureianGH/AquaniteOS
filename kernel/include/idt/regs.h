#pragma once
#include <stdint.h>
#ifdef __cplusplus
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
#else
struct registers_t {
  volatile unsigned long long r15;
  volatile unsigned long long r14;
  volatile unsigned long long r13;
  volatile unsigned long long r12;
  volatile unsigned long long r11;
  volatile unsigned long long r10;
  volatile unsigned long long r9;
  volatile unsigned long long r8;
  volatile unsigned long long rsi;
  volatile unsigned long long rdi;
  volatile unsigned long long rbp;
  volatile unsigned long long rdx;
  volatile unsigned long long rcx;
  volatile unsigned long long rbx;
  volatile unsigned long long rax;
  volatile unsigned long long core;
  volatile unsigned long long isrNumber;
  volatile unsigned long long errorCode;
  volatile unsigned long long rip;
  volatile unsigned long long cs;
  volatile unsigned long long rflags;
  volatile unsigned long long rsp;
  volatile unsigned long long ss;
} __attribute__((packed));
#endif