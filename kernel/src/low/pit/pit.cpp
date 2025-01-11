#include <pit/pit.h>
#include <idt/isr.h>
#include <flanterm/flantermglobal.h>
#include <sched/sched.h>
#include <sched/sched.h>

spinlock_t pit_lock; // Define a spinlock for synchronization

void PITSetFrequency(uint16_t frequency)
{
    spinlock_acquire(&pit_lock); // Acquire the spinlock
    uint16_t divisor = PIT_FREQUENCY / frequency;
    outb(PIT_COMMAND, PIT_COMMAND_CHANNEL0 | PIT_COMMAND_MODE3 | PIT_COMMAND_BINARY);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
    spinlock_release(&pit_lock); // Release the spinlock
}

Vector<uint64_t> PITSleepingProcesses;

void PITSleep(uint64_t ms)
{
    spinlock_acquire(&pit_lock); // Acquire the spinlock
    // Add the current process to the sleeping processes vector
    PITSleepingProcesses.PushBack(ms);
    // Implement the sleep logic here
    spinlock_release(&pit_lock); // Release the spinlock
}

void PITHandler(registers_t *r)
{
    spinlock_acquire(&pit_lock); // Acquire the spinlock
    for (size_t i = 0; i < PITSleepingProcesses.Length(); ++i) {
        if (PITSleepingProcesses[i] > 0) {
            PITSleepingProcesses[i]--;
        }
        if (PITSleepingProcesses[i] == 0) {
            // Wake up the process
            // Implement the logic to wake up the process here
            PITSleepingProcesses.Erase(i);
            --i; // Adjust the index after removal
        }
    }
    spinlock_release(&pit_lock); // Release the spinlock
}

void PITInit()
{
    spinlock_init(&pit_lock); // Initialize the spinlock
    PITSetFrequency(1000);
    PITSleepingProcesses = Vector<uint64_t>();
    lprintf(logging_level::OK, "PIT initialized.\n");
}