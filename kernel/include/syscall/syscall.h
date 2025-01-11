#pragma once
#include <stdint.h>
typedef enum
{
    SYSTEM_IO,      // I/O
    SYSTEM_PROC,    // Process Management
    SYSTEM_MEM,     // Memory Management
    SYSTEM_FS,      // File System
    SYSTEM_NET,     // Networking
    SYSTEM_DEV,     // Device Management
    SYSTEM_TIME,    // Time Management
    SYSTEM_USER,    // User Management
    SYSTEM_SYS,     // System Management
    SYSTEM_ERR,     // Error Handling
    SYSTEM_OTHER    // Other
} syscall_rax_t;

typedef enum
{
    SYSTEM_IO_CONSOLE_STDOUT,   // Print to console
    SYSTEM_IO_CONSOLE_STDIN,    // Read from console
    SYSTEM_IO_CONSOLE_CLEAR,    // Clear console
    SYSTEM_IO_CONSOLE_CURSOR   // Set console cursor
} syscall_system_io_t;

typedef enum
{
    SYSTEM_PROC_CREATE,     // Create process
    SYSTEM_PROC_TERMINATE,  // Terminate process
    SYSTEM_PROC_GET_PID,    // Get process ID
    SYSTEM_PROC_GET_NAME,   // Get process name
    SYSTEM_PROC_GET_COUNT   // Get process count
} syscall_system_proc_t;

typedef enum
{
    SYSTEM_MEM_ALLOC,   // Allocate memory
    SYSTEM_MEM_FREE,    // Free memory
    SYSTEM_MEM_READ,    // Read memory
    SYSTEM_MEM_WRITE    // Write memory
} syscall_system_mem_t;

typedef enum
{
    SYSTEM_FS_OPEN,     // Open file
    SYSTEM_FS_CLOSE,    // Close file
    SYSTEM_FS_READ,     // Read file
    SYSTEM_FS_WRITE,    // Write file
    SYSTEM_FS_SEEK,     // Seek file
    SYSTEM_FS_TELL,     // Tell file
    SYSTEM_FS_EOF,      // End of file
    SYSTEM_FS_SIZE,     // File size
    SYSTEM_FS_DELETE    // Delete file
} syscall_system_fs_t;

typedef enum
{
    SYSTEM_NET_CONNECT,     // Connect to network
    SYSTEM_NET_DISCONNECT,  // Disconnect from network
    SYSTEM_NET_SEND,        // Send data
    SYSTEM_NET_RECV,        // Receive data
    SYSTEM_NET_LISTEN,      // Listen for connections
    SYSTEM_NET_ACCEPT       // Accept connection
} syscall_system_net_t;

typedef enum
{
    SYSTEM_DEV_OPEN,    // Open device
    SYSTEM_DEV_CLOSE,   // Close device
    SYSTEM_DEV_READ,    // Read from device
    SYSTEM_DEV_WRITE    // Write to device
} syscall_system_dev_t;

typedef enum
{
    SYSTEM_TIME_GET,    // Get time
    SYSTEM_TIME_SET,     // Set time
    SYSTEM_TIME_SLEEP,  // Sleep
    SYSTEM_TIME_MS_SINCE_BOOT  // Milliseconds since boot
} syscall_system_time_t;

typedef enum
{
    SYSTEM_USER_CREATE,     // Create user
    SYSTEM_USER_DELETE,     // Delete user
    SYSTEM_USER_GET_ID,     // Get user ID
    SYSTEM_USER_GET_NAME    // Get user name
} syscall_system_user_t;

typedef enum
{
    SYSTEM_SYS_INSTALLABLE,
    SYSTEM_SYS_REBOOT,     // Reboot system
    SYSTEM_SYS_SHUTDOWN,   // Shutdown system
    SYSTEM_SYS_HALT        // Halt system
} syscall_system_sys_t;

typedef enum
{
    SYSTEM_INSTALLABLE_KEYBOARD
} syscall_system_sys_installable_t;

typedef enum
{
    SYSTEM_ERR_GET,    // Get error
    SYSTEM_ERR_SET     // Set error
} syscall_system_err_t;

typedef struct
{
    syscall_rax_t rax;
    union
    {
        syscall_system_io_t io;
        syscall_system_proc_t proc;
        syscall_system_mem_t mem;
        syscall_system_fs_t fs;
        syscall_system_net_t net;
        syscall_system_dev_t dev;
        syscall_system_time_t time;
        syscall_system_user_t user;
        syscall_system_sys_t sys;
        syscall_system_err_t err;
    };
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
} syscall_t;

void syscall_install(uint8_t lane, char (*getchar_)());