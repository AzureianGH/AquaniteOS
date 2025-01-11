#include <sched/sched.h>
#include <flanterm/flantermglobal.h>
#include <idt/idt.h>
#include <shared.h>

void* __dso_handle = (void*)&__dso_handle;
uint64_t sched_pid_next = 0, sched_pid_current = 0;
extern Vector<process_t*> processes_in_system;
process_t* next_proc = nullptr;
spinlock_t sched_lock;
void sched_init()
{
    sched_pid_next = 0;
    sched_pid_current = 0;
    spinlock_init(&sched_lock);
    lprintf(logging_level::OK, "Scheduler initialized.\n");
}
extern void main_process();
extern void main_shell_ready();

process_t* get_next_process()
{
    spinlock_acquire(&sched_lock);
    size_t process_count = get_process_count();
    if (process_count == 0)
    {
        lprintf(logging_level::ERROR, "No processes available to schedule.\n");
        spinlock_release(&sched_lock);
        return nullptr;
    }
    for (size_t i = 0; i < process_count; ++i)
    {
        sched_pid_next = (sched_pid_next + 1) % process_count;
        process_t* next_process = processes_in_system[sched_pid_next];

        if (next_process != nullptr)
        {
            spinlock_release(&sched_lock);
            return next_process;
        }
    }

    lprintf(logging_level::ERROR, "No valid processes found.\n");
    spinlock_release(&sched_lock);
    return nullptr;
}

bool is_swap_allowed = true;
uint32_t sched_lock_num = 0;
void lock_scheduler()
{
    is_swap_allowed = false;
    sched_lock_num++;
}
void unlock_scheduler()
{
    sched_lock_num--;
    if (sched_lock_num == 0)
    {
        is_swap_allowed = true;
    }
}

void swap_proc()
{
}



void scheduler(registers_t* r)
{
    if(!is_swap_allowed){return;}
    // Get the next process
    process_t* next_process = get_next_process();
    if (!next_process)
    {
        lprintf(logging_level::ERROR, "No process to schedule.\n");
        return;
    }
    if (next_process->state == PROCESS_TERMINATED)
    {
        terminate_process(next_process);
        return;
    }
    //check if the next process is the same as the current process
    if (next_process == next_proc)
    {
        return;
    }
    // Set the next process as the current process
    next_proc = next_process;
    sched_pid_current = sched_pid_next;

    uint64_t flags = r->rflags; // Preserve certain flags
    *r = next_proc->context;
    r->rflags = flags;
}

void spinlock_init(spinlock_t* lock)
{
    lock->lock = 0;  // Initialize the lock to unlocked state (0)
}

void spinlock_acquire(spinlock_t* lock)
{
    // Spin until the lock is acquired
    while (__sync_lock_test_and_set(&lock->lock, 1))
    {
        // Do nothing
    }
}

bool is_spinlock_owned(spinlock_t* lock)
{
    //check if spinlock has owner
    return lock->lock;
}

void spinlock_release(spinlock_t* lock)
{
    // Release the lock
    __sync_lock_release(&lock->lock);
}