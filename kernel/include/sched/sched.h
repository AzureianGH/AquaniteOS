#pragma once

#include "proc.h"
#include <idt/regs.h>

void sched_init();
process_t* get_next_process();
void lock_scheduler();
void unlock_scheduler();

void scheduler(struct registers_t* r);
void swap_proc(registers_t* r);

// SPINLOCKS //
typedef struct {
    volatile int lock;  //(0 = unlocked, 1 = locked)
} spinlock_t;



void spinlock_acquire(spinlock_t* lock);

void spinlock_release(spinlock_t* lock);

void spinlock_init(spinlock_t* lock);