#include <pit/pit.h>
#include <idt/isr.h>
#include <flanterm/flantermglobal.h>
#include <sched/sched.h>
void PITSetFrequency(uint16_t frequency)
{
    uint16_t divisor = PIT_FREQUENCY / frequency;
    outb(PIT_COMMAND, PIT_COMMAND_CHANNEL0 | PIT_COMMAND_MODE3 | PIT_COMMAND_BINARY);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}
Vector<uint64_t> PITSleepingProcesses;
void PITSleep(uint64_t ms)
{
    uint64_t ticks = ms * 1000 / 55;
    get_current_process()->ticks = 0;
    PITSleepingProcesses.PushToBack(ticks);
    while (get_current_process()->ticks < ticks)
    {
        asm("hlt");
    }
    PITSleepingProcesses.EraseValue(ticks);
}

void PITHandler(registers_t *r)
{
    if (get_process_count() == 0)
    {
        return;
    }
    //each process has its own tick count
    get_current_process()->ticks++;
}


void PITInit()
{
    //tell the PIC to
    PITSetFrequency(1000);
    PITSleepingProcesses = Vector<uint64_t>();
    lprintf(logging_level::OK, "PIT initialized.\n");
}