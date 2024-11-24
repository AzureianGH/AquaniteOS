#include <pit/pit.h>
#include <idt/isr.h>
#include <flanterm/flantermglobal.h>

uint64_t ticks = 0;

void PITSetFrequency(uint16_t frequency)
{
    uint16_t divisor = PIT_FREQUENCY / frequency;
    outb(PIT_COMMAND, PIT_COMMAND_CHANNEL0 | PIT_COMMAND_MODE3 | PIT_COMMAND_BINARY);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void PITSleep(uint64_t ms)
{
    uint64_t end = ticks + ms;
    while (ticks < end)
    {
        asm("hlt");
    }
}

void PITHandler(registers_t *r)
{
    ticks++;
}


void PITInit()
{
    //tell the PIC to
    PITSetFrequency(1000);
    lprintf(logging_level::OK, "PIT initialized.\n");
}