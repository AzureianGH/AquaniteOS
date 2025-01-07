#include <sched/proc.h>
#include <memory/dlmalloc.h>
#include <shared.h>
#include <flanterm/flantermglobal.h>
int64_t process_count, current_pid;
Vector<process_t*> processes_in_system;
void process_init()
{
    process_count = 0;
    current_pid = 0;
    processes_in_system = Vector<process_t*>();
    lprintf(logging_level::OK, "Process system initialized.\n");
}

process_t* process_create(void(*base)(void))
{
    process_t* new_process = (process_t*)malloc(sizeof(process_t));
    new_process->pid = current_pid;
    new_process->state = PROCESS_READY;
    new_process->base = (uint64_t*)base;
    new_process->context.rax = 0;
    new_process->context.rbx = 0;
    new_process->context.rcx = 0;
    new_process->context.rdx = 0;
    new_process->context.rsi = 0;
    new_process->context.rdi = 0;
    new_process->context.rbp = 0;
    new_process->context.r8 = 0;
    new_process->context.r9 = 0;
    new_process->context.r10 = 0;
    new_process->context.r11 = 0;
    new_process->context.r12 = 0;
    new_process->context.r13 = 0;
    new_process->context.r14 = 0;
    new_process->context.r15 = 0;
    new_process->context.cs = 0x08;
    new_process->context.ss = 0x10;
    new_process->stack_rsp = (uint64_t*)malloc(4096);
    new_process->context.rsp = (uint64_t)new_process->stack_rsp + 4096;
    new_process->context.rip = (uint64_t)base;
    processes_in_system.PushToBack(new_process);
    process_count++;
    current_pid++;
    return new_process;
}

uint64_t get_process_count()
{
    return process_count;
}

void terminate_process(process_t* proc)
{
    free(proc->base);
    process_count--;
    processes_in_system.EraseValue(proc);
    free(proc);
}

uint64_t get_current_pid()
{
    return current_pid;
}

process_t* get_current_process()
{
    return processes_in_system[process_count - 1];
}