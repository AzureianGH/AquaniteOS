#pragma once
// pit

#include <stdint.h>
#include <io/io.h>
#include <idt/regs.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_COMMAND 0x43

#define PIT_COMMAND_CHANNEL0 0
#define PIT_COMMAND_CHANNEL1 1
#define PIT_COMMAND_CHANNEL2 2

#define PIT_COMMAND_LATCH 0
#define PIT_COMMAND_MODE0 0
#define PIT_COMMAND_MODE1 1
#define PIT_COMMAND_MODE2 2
#define PIT_COMMAND_MODE3 3
#define PIT_COMMAND_MODE4 4
#define PIT_COMMAND_MODE5 5

#define PIT_COMMAND_BINARY 0
#define PIT_COMMAND_BCD 1

#define PIT_COMMAND_READBACK 0
#define PIT_COMMAND_READBACK_LATCH 1
#define PIT_COMMAND_READBACK_COUNT 2
#define PIT_COMMAND_READBACK_STATUS 3

#define PIT_FREQUENCY 1193182 // 1.193182 MHz

void PITHandler(registers_t *r);
void PITInit();
void PITSetFrequency(uint16_t frequency);
void PITSleep(uint64_t ms);