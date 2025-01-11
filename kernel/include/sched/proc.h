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

typedef enum
{
    USER_INITIATED, // Kill Proc
    SYSTEM_INITIATED, // Restart Proc
    KERNEL_INITIATED // Halt System
} process_importance_t;

enum class process_flags_ec
{
    MARKED_FOR_TERMINATION = 1,
    KERNEL_MODE = 2,
    IS_VIRTUAL = 4
};

typedef struct
{
    uint64_t pid;
    process_state_t state;
    struct registers_t context;
    uint64_t* base;
    uint64_t* stack_rsp;
    uint64_t ticks;
    process_importance_t importance;
    char* name;
    union
    {
        uint16_t marked_for_termination : 1;
        uint16_t kernel_mode : 1;
        uint16_t is_virtual : 1;
        uint16_t reserved : 13;
    };
     
} process_t;




void process_init();
process_t* process_create(void(*base)(void), process_importance_t importance, char* name, int flags);
process_t* get_current_process();
uint64_t get_process_count();
uint64_t get_current_pid();
void terminate_process(process_t* proc);
void terminate_process_pid(uint64_t pid);
char* get_process_name(uint64_t pid);