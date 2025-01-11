#include <syscall/syscall.h>
#include <idt/regs.h>
#include <sched/proc.h>
#include <sched/sched.h>
#include <flanterm/flantermglobal.h>
#include <shared.h>
#include <idt/isr.h>
#include <memory/dlmalloc.h>
#include <ps2/keyboard.h>
char (*getchar)() = nullptr;

void syscall_handler(registers_t* r);
void syscall_install(uint8_t lane, char (*getchar_)())
{
    // Set the syscall gate
    lprintf(logging_level::INFO, "Setting syscall gate for lane %d\n", lane);
    install_isr_at(lane, syscall_handler);
    getchar = getchar_;
    lprintf(logging_level::OK, "Syscall installed.\n");
}
void syscall_handler(registers_t* r)
{
    asm volatile("cli");
    
    if (r->rax == SYSTEM_IO)
    {
        if (r->rbx == SYSTEM_IO_CONSOLE_STDOUT)
        {
            char* buffer = (char*)r->rcx;
            uint64_t length = r->rdx;

            for (uint64_t i = 0; i < length; i++)
            {
                printf("%c", buffer[i]);
            }
        }
        else if (r->rbx == SYSTEM_IO_CONSOLE_STDIN)
        {
            char* buffer = (char*)r->rcx;
            uint64_t length = r->rdx;

            for (uint64_t i = 0; i < length; i++)
            {
                buffer[i] = getchar();
            }
        }
        else if (r->rbx == SYSTEM_IO_CONSOLE_CLEAR)
        {
            clear_screen();
        }
        else if (r->rbx == SYSTEM_IO_CONSOLE_CURSOR)
        {
            set_cursor(r->rcx, r->rdx);
        }
    }
    else if (r->rax == SYSTEM_PROC)
    {
        if (r->rbx == SYSTEM_PROC_CREATE)
        {
            void (*base)() = (void (*)())r->rcx;
            process_importance_t importance = (process_importance_t)r->rdx;
            char* name = (char*)r->rsi;

            process_t* new_process = process_create(base, importance, name, r->r8);
            r->rax = new_process->pid;
        }
        else if (r->rbx == SYSTEM_PROC_TERMINATE)
        {
            uint64_t pid = r->rcx;
            process_t* proc = nullptr;
            //use terminate_process_pid
            terminate_process_pid(pid);
        }
        else if (r->rbx == SYSTEM_PROC_GET_PID)
        {
            r->rax = get_current_pid();
        }
        else if (r->rbx == SYSTEM_PROC_GET_NAME)
        {
            uint64_t pid = r->rcx;
            process_t* proc = nullptr;
            //use functiojn get_process_name
            char* name = get_process_name(pid);
            r->rax = (uint64_t)name;
        }
        else if (r->rbx == SYSTEM_PROC_GET_COUNT)
        {
            r->rax = get_process_count();
        }
    }
    else if (r->rax == SYSTEM_MEM)
    {
        if (r->rbx == SYSTEM_MEM_ALLOC)
        {
            uint64_t size = r->rcx;
            r->rax = (uint64_t)malloc(size);
        }
        else if (r->rbx == SYSTEM_MEM_FREE)
        {
            void* ptr = (void*)r->rcx;
            free(ptr);
        }
        else if (r->rbx == SYSTEM_MEM_READ)
        {
            void* ptr = (void*)r->rcx;
            uint64_t size = r->rdx;
            void* buffer = (void*)r->rsi;
            memcpy(buffer, ptr, size);
        }
        else if (r->rbx == SYSTEM_MEM_WRITE)
        {
            void* ptr = (void*)r->rcx;
            uint64_t size = r->rdx;
            void* buffer = (void*)r->rsi;
            memcpy(ptr, buffer, size);
        }
    }
    else if (r->rax == SYSTEM_SYS)
    {
        if (r->rbx == SYSTEM_SYS_INSTALLABLE)
        {
            if (r->rcx == SYSTEM_INSTALLABLE_KEYBOARD)
            {
                AddKeyboardInterrupt((void (*)(key_t))r->rdx);
            }
        }
    }
    asm volatile("sti");
}

/*
asm code to get proc count
mov rax, 0x1
mov rbx, 0x2
int 0x80
*/