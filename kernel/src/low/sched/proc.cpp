#include <sched/proc.h>
#include <memory/dlmalloc.h>
#include <shared.h>
#include <flanterm/flantermglobal.h>
#include <sched/sched.h>
int64_t process_count, current_pid;
Vector<process_t*> processes_in_system;
spinlock_t process_lock;
void process_init()
{
    process_count = 0;
    current_pid = 0;
    processes_in_system = Vector<process_t*>();
    spinlock_init(&process_lock);
    lprintf(logging_level::OK, "Process system initialized.\n");
}

process_t* process_create(void(*base)(void), process_importance_t importance, char* name, int flags)
{
    spinlock_acquire(&process_lock);
    process_t* new_process = (process_t*)malloc(sizeof(process_t));
    new_process->name = name;
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
    new_process->context.r8  = 0;
    new_process->context.r9  = 0;
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
    new_process->importance = importance;
    processes_in_system.PushToBack(new_process);
    process_count++;
    current_pid++;
    if (has_flag(flags, (int)process_flags_ec::KERNEL_MODE))
    {
        new_process->kernel_mode = 1;
    }
    if (has_flag(flags, (int)process_flags_ec::IS_VIRTUAL))
    {
        new_process->is_virtual = 1;
    }
    if (has_flag(flags, (int)process_flags_ec::MARKED_FOR_TERMINATION)) // for the weirdos that set the process to terminate upon creation
    {
        new_process->marked_for_termination = 1;
    }
    spinlock_release(&process_lock);
    return new_process;
}

uint64_t get_process_count()
{
    return process_count;
}

void process_exit()
{
    spinlock_acquire(&process_lock);
    process_t* proc = get_current_process();
    proc->state = PROCESS_TERMINATED;
    proc->marked_for_termination = true;
    spinlock_release(&process_lock);
}

void terminate_process(process_t* proc)
{
    spinlock_acquire(&process_lock);
    free(proc->base);
    process_count--;
    processes_in_system.EraseValue(proc);
    free(proc);
    spinlock_release(&process_lock);
}

void terminate_process_pid(uint64_t pid)
{

    for (size_t i = 0; i < processes_in_system.Length(); i++)
    {
        if (processes_in_system[i]->pid == pid)
        {
            terminate_process(processes_in_system[i]);
            return;
        }
    }
}

uint64_t get_current_pid()
{
    return current_pid;
}

process_t* get_current_process()
{
    return processes_in_system[process_count - 1];
}

char* get_process_name(uint64_t pid)
{
    for (size_t i = 0; i < processes_in_system.Length(); i++)
    {
        if (processes_in_system[i]->pid == pid)
        {
            return processes_in_system[i]->name;
        }
    }
    return nullptr;
}