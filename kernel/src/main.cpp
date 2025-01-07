/// ---------- INCLUDES ---------- ///
#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <flanterm/flanterm.h>
#include <flanterm/flantermglobal.h>
#include <flanterm/backends/fb.h>
#include <memory/dlmalloc.h>
#include <memory/heap.h>
#include <gdt/gdt.h>
#include <idt/isr.h>
#include <pit/pit.h>
#include <io/io.h>
#include <sched/sched.h>
#include <sched/proc.h>
#include <memory/gccmemory.h>
#include <string/string.h>
#include <memory/paging/heap.h>
#include <memory/paging/vmm.h>
#include <memory/paging/physical.h>
#include <memory/paging/common_memory.h>
#include <ps2/keyboard.h>
#include <uacpi/uacpi.h>
#include <uacpi/event.h>
#include <amfs/amfs_dec.h>
/// ------------------------------ ///

/// ---------- DEFINES ---------- ///
namespace {__attribute__((used, section(".requests")))volatile LIMINE_BASE_REVISION(2);}

namespace {
__attribute__((used, section(".requests")))
volatile limine_boot_time_request boot_time_request = {
    .id = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0,
    .response = nullptr
};
__attribute__((used, section(".requests")))
volatile limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0,
    .response = nullptr
};
__attribute__((used, section(".requests")))
volatile limine_dtb_request dtb_request = {
    .id = LIMINE_DTB_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};
__attribute__((used, section(".requests")))
volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};
__attribute__((used, section(".requests")))
volatile limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
    .response = nullptr
};
__attribute__((used, section(".requests")))
volatile limine_kernel_address_request limine_kernel_address = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST, 
    .revision = 0, 
    .response = nullptr
};
__attribute__((used, section(".requests")))
volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};
}
/// ---------------------------- ///

/// ---------- GLOBAL INITS ---------- ///
namespace {__attribute__((used, section(".requests_start_marker")))volatile LIMINE_REQUESTS_START_MARKER;__attribute__((used, section(".requests_end_marker")))volatile LIMINE_REQUESTS_END_MARKER;}
namespace {void halt() {for (;;) asm ("hlt");}void hcf() {asm ("cli");for (;;) {asm ("hlt");}}}
extern "C" {int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }void __cxa_pure_virtual() { hcf(); }}
extern void (*__init_array[])();
extern void (*__init_array_end[])();
/// ---------------------------------- ///

/// ---------- GLOBAL VARS ---------- ///
limine_framebuffer_response *limine_framebuffer_resp;
limine_kernel_address_response *limine_kernel_addy;
limine_memmap_response *limine_memmap_us;
limine_hhdm_response *limine_hhdm_resp;
limine_rsdp_response *limine_rsdp_resp;
limine_framebuffer *framebuffer;
extern uint64_t kernel_start;
extern uint64_t kernel_end;
static uint8_t bwa_amfs[]  = {
  0xf5, 0xac, 0x00, 0x00, 0x01, 0x00, 0x16, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x02, 0x00, 
  0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x74, 0x65, 0x73, 0x74, 0x66, 0x69, 0x6c, 0x65, 0x73, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 
  0xff, 0xff, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2e, 0x74, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x62, 0x77, 
  0x61, 0x20, 0x62, 0x77, 0x61, 0x20, 0x3a, 0x33
};
/// ---------- GLOBAL FUNCS ---------- ///
void main_process();
void handle_shell_input(key_t key);
extern void kernel_main() {
    
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {hcf();}
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++) {__init_array[i]();}
    if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1) {hcf();}
    else { framebuffer = framebuffer_request.response->framebuffers[0]; }
    if (memmap_request.response == nullptr) {hcf();}
    limine_memmap_us = memmap_request.response;
    if (hhdm_request.response == nullptr) {hcf();}
    limine_hhdm_resp = hhdm_request.response;
    if (limine_kernel_address.response == nullptr) {hcf();}
    limine_kernel_addy = limine_kernel_address.response;
    if (rsdp_request.response == nullptr) {hcf();}
    limine_rsdp_resp = rsdp_request.response;
    

    struct flanterm_context *ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        (uint32_t*)framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size, framebuffer->red_mask_shift,
        framebuffer->green_mask_size, framebuffer->green_mask_shift,
        framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    ); flanterm_set_instance(ft_ctx);
    lprintf(logging_level::OK, "Framebuffer initialized.\n");
    gdt_init();
    set_heap_start(hhdm_request.response->offset);
    set_up_common_mem(limine_hhdm_resp, limine_memmap_us, limine_kernel_addy);
    process_init();
    sched_init();
    process_create(main_process);
    KeyboardInit();
    AddKeyboardInterrupt(handle_shell_input);
    InitializeISR();
    InitializeIDT();
    initialize_phys_memory();
    initialize_virtual_memory();
    initialize_heap();
    PITInit();
    set_uacpi_rsdp(limine_rsdp_resp->address);
    //uacpi_status ret = uacpi_initialize(0);
    ISRIgnoreFaults();
    //int 0x0
    asm volatile("int $0x0");
    ISRCatchFaults();
    
    lprintf(logging_level::OK, "Kernel initialized.\n");
    lprintf(logging_level::INFO, "Waiting for scheduler handoff...\n");
    EnableInterrupts();
    while (true); // This point will be reached until the scheduler takes control. Then this will never be reached again.
    halt(); // This should never be reached. If it is, halt the system. (We say "if" since code is never perfect)
}

uint8_t input_buffer[1024];
uint16_t input_buffer_index = 0;
bool ready_for_input = false;
bool looped = false;
bool shift_pressed = false; // Track if Shift key is pressed
amfs_handle_t amfs;
void clear_input_buffer() {
    memset(input_buffer, 0, sizeof(input_buffer));
    input_buffer_index = 0;
}
#define MAX_ARGS 10 // Max number of arguments

void test_proc_fail()
{
    int a = 3;
    int b = 5;
    int c = a / (b - 5); // This will cause a divide by zero error
    printf("c: %d\n", c); // This will never be reached
}

char *args[MAX_ARGS];
void main_shell_ready();
registers_t *r_l_int;
void handle_command() {
    // Null-terminate the input buffer for safe string operations
    input_buffer[input_buffer_index] = '\0';

    // Tokenize the input buffer into arguments (split by spaces)
    int argc = 0;
    char *token = strtok((char *)input_buffer, " ");  // Split by spaces
    while (token != NULL && argc < MAX_ARGS) {
        args[argc++] = token;
        token = strtok(NULL, " "); // Get the next token
    }

    // Check the command and respond
    if (argc > 0) {
        if (strcmp(args[0], "amfs") == 0) {
            if (argc > 1)
            {
                if (strcmp(args[1], "init") == 0)
                {
                    amfs = amfs_init(bwa_amfs);
                    if (amfs == NULL)
                    {
                        printf("Failed to initialize AMFS.\n");
                    }
                    else
                    {
                        printf("AMFS initialized.\n");
                    }
                }
                else if (strcmp(args[1], "read") == 0)
                {
                    if (argc > 2)
                    {
                        uint32_t size;
                        uint8_t *data = amfs_read_file(amfs, args[2], &size);
                        if (data == NULL)
                        {
                            printf("Failed to read file.\n");
                        }
                        else
                        {
                            printf("%s\n", data);
                        }
                    }
                    else
                    {
                        printf("Usage: amfs read [file]\n");
                    }
                }
                else if (strcmp(args[1], "size") == 0)
                {
                    if (argc > 2)
                    {
                        int64_t size = amfs_get_file_size(amfs, args[2]);
                        if (size == -1)
                        {
                            printf("File not found.\n");
                        }
                        else
                        {
                            printf("File size: %d bytes\n", size);
                        }
                    }
                    else
                    {
                        printf("Usage: amfs size [file]\n");
                    }
                }
                else if (strcmp(args[1], "list") == 0)
                {
                    amfs_list_directory(amfs, "/");
                }
                else
                {
                    printf("Unknown subcommand: %s\n", args[1]);
                }
            }
            else
            {
                printf("Usage: amfs [init|read|size]\n");
            }
        }
        else if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; i < argc; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
        } 

        else if (strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            main_shell_ready(); // Reset the shell state
            terminate_process(get_current_process()); // Terminate the current process
            halt(); // Halt the system (if applicable)
        }

        else if (strcmp(args[0], "test") == 0) {
            process_create(test_proc_fail);
        }
        else if (strcmp(args[0], "clear") == 0) {
            printf("\033[2J\033[H"); // Clear the terminal
        } 
        else if (strcmp(args[0], "help") == 0) {
            printf("Available commands:\n");
            printf("echo [text] - Print text to the terminal\n");
            printf("clear - Clear the terminal\n");
            printf("help - Display this help message\n");
            printf("capture [next|start|stop|print] - Capture and print the last interrupt\n");
        }
        else if (strcmp(args[0], "capture") == 0) {
        if (argc > 1) {
            if (strcmp(args[1], "next") == 0) {
                if (argc > 2) {
                    if (strcmp(args[2], "int") == 0) {
                        if (IDT_registers_to_capture()) {
                            r_l_int = IDT_get_last_registers();
                            printf("Last interrupt: 0x%x\n", r_l_int->isrNumber);
                        } else {
                            printf("No interrupts captured yet.\n");
                        }
                    }
                }
            } 
            else if (strcmp(args[1], "print") == 0) {
                if (argc > 2) {
                    if (strcmp(args[2], "int") == 0) {
                        if (r_l_int != nullptr) {
                            //full dump
                            printf("\033[0mInterrupt number: 0x%l\n", r_l_int->isrNumber);
                            printf("\033[0mRIP: 0x%i    RSP:  0x%i\n", r_l_int->rip, r_l_int->rsp);
                            printf("\033[0mRAX: 0x%i    RBX:  0x%i\n", r_l_int->rax, r_l_int->rbx);
                            printf("\033[0mRCX: 0x%i    RDX:  0x%i\n", r_l_int->rcx, r_l_int->rdx);
                            printf("\033[0mRSI: 0x%i    RDI:  0x%i\n", r_l_int->rsi, r_l_int->rdi);
                            printf("\033[0mRBP: 0x%i    R8:   0x%i\n", r_l_int->rbp, r_l_int->r8);
                            printf("\033[0mR9 : 0x%i    R10:  0x%i\n", r_l_int->r9, r_l_int->r10);
                            printf("\033[0mR11: 0x%i    R12:  0x%i\n", r_l_int->r11, r_l_int->r12);
                            printf("\033[0mR13: 0x%i    R14:  0x%i\n", r_l_int->r13, r_l_int->r14);
                            printf("\033[0mR15: 0x%i    RFLG: 0x%i\n", r_l_int->r15, r_l_int->rflags);
                            printf("\033[0mCS:  0x%i    SS:   0x%i\n", r_l_int->cs, r_l_int->ss);
                        } else {
                            printf("No interrupts captured yet.\n");
                        }
                    }
                }
            } 
            else {
                printf("Unknown subcommand: %s\n", args[1]);
            }
        } 
        else {
            printf("Usage: capture [next|start|stop|print]\n");
        }
    }

        else {
            printf("Unknown command: %s\n", args[0]);
        }
    } else {
        printf("No command entered.\n");
    }

    // Clear the buffer and reset the arguments
    ready_for_input = true;
    looped = false;
    clear_input_buffer();
}

void handle_shell_input(key_t key) {
    
    if (key.released) {
        // Reset shift state when released
        if (key.scancode == 0xAA || key.scancode == 0x36) {
            shift_pressed = false;
            
        }
        return; // Ignore key releases
    }

    // Handle Shift key press
    if (key.scancode == 0x2A || key.scancode == 0x36) {
        shift_pressed = true;
        return; // Don't process Shift key as input
    }

    // Handle Backspace
    if (key.scancode == 0x0E) {
        if (input_buffer_index > 0) {
            input_buffer_index--;
            input_buffer[input_buffer_index] = '\0';
            printf("\b \b"); // Move cursor back, erase character, move cursor back again
        }
    }
    // Handle Enter
    else if (key.scancode == 0x1C) {
        ready_for_input = false; // Stop accepting input until the command is processed
        printf("\n"); // Move to the next line
        handle_command(); // Process the command
    }
    // Handle other keys
    else {
        // Define standard ASCII characters for scancodes
        static const char scancode_to_ascii[128] = {
            0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
            '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
            0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
            0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
            '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
        };

        // Define shifted ASCII characters (e.g., Shift+1 -> '!', Shift+2 -> '@', etc.)
        static const char shifted_scancode_to_ascii[128] = {
            0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 
            '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
            0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
            0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
            '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
        };

        // Use the shifted mapping if Shift is pressed, otherwise use the normal one
        char ascii_char = 0;
        if (key.scancode < 128) {
            if (shift_pressed) {
                ascii_char = shifted_scancode_to_ascii[key.scancode];
            } else {
                ascii_char = scancode_to_ascii[key.scancode];
            }
        }

        // If a valid character is found, add it to the input buffer
        if (ascii_char != 0 && input_buffer_index < sizeof(input_buffer) - 1) {
            input_buffer[input_buffer_index++] = ascii_char;
            printf("%c", ascii_char); // Echo the character to the terminal
        }
    }
}

bool mainprocstartfirst = false;
extern void main_process() {
    if (!mainprocstartfirst)
    {
        lprintf(logging_level::OK, "Scheduler control given; main process started.\n");
        ready_for_input = true;
        printf("Welcome to the Aquanite shell!\n");
        mainprocstartfirst = true;
    }
    ready_for_input = true;
    while (true) {
        // Main loop does nothing; input is interrupt-driven
        if (ready_for_input && !looped) {
            printf("\033[0maqua > ");
            looped = true;
        }

        // Reset the looped flag when command is processed and ready for input again
        if (!ready_for_input) {
            looped = false;
        }
    }
}

extern void main_shell_ready()
{
    //set all bools
    looped = false;
    mainprocstartfirst = false;
    ready_for_input = false;
    shift_pressed = false;
    //clear the input buffer
    clear_input_buffer();
    //reset the arguments
    for (int i = 0; i < MAX_ARGS; i++) {
        args[i] = nullptr;
    }
}

/// --------------------------------- ///