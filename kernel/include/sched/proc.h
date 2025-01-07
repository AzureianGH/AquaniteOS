#pragma once
#include <stdint.h>
#include <idt/regs.h>

typedef enum
{
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

typedef struct
{
    uint64_t pid;
    process_state_t state;
    struct registers_t context;
    uint64_t* base;
    uint64_t* stack_rsp;
    uint64_t ticks;
} process_t;




void process_init();
process_t* process_create(void(*base)(void));
process_t* get_current_process();
uint64_t get_process_count();
uint64_t get_current_pid();
void terminate_process(process_t* proc);