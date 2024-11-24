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
limine_memmap_response *limine_memmap_us;
limine_framebuffer_response *limine_framebuffer_resp;
extern uint64_t kernel_start;
extern uint64_t kernel_end;
limine_framebuffer* framebuffer;

/// ---------- GLOBAL FUNCS ---------- ///
void test_ps2_keyboard(key_t key);
void main_process();
extern void kernel_main() {
    
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {hcf();}
    for (size_t i = 0; &__init_array[i] != __init_array_end; i++) {__init_array[i]();}
    if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1) {hcf();}
    else { framebuffer = framebuffer_request.response->framebuffers[0]; }

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
    process_init();
    sched_init();
    process_create(main_process);
    
    InitializeISR();
    InitializeIDT();
    PITInit();
    lprintf(logging_level::OK, "Kernel initialized.\n");
    lprintf(logging_level::INFO, "Waiting for scheduler handoff...\n");
    EnableInterrupts();
    while (true); // This point will be reached until the scheduler takes control. Then this will never be reached again.
    halt(); // This should never be reached. If it is, halt the system. (We say "if" since code is never perfect)
}

uint8_t input_buffer[1024];
uint16_t input_buffer_index = 0;

void clear_input_buffer() {
    memset(input_buffer, 0, sizeof(input_buffer));
    input_buffer_index = 0;
}

void handle_command() {
    // Null-terminate the input buffer for safe string operations
    input_buffer[input_buffer_index] = '\0';

    // Check the command and respond
    if (strcmp((char *)input_buffer, "hello") == 0) {
        printf("Hello, world!\n");
    } else {
        printf("Unknown command: %s\n", input_buffer);
    }

    // Clear the buffer after handling the command
    clear_input_buffer();
}

void handle_shell_input(key_t key) {
    if (key.released) {
        return; // Ignore key releases
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
        printf("\n"); // Move to the next line
        handle_command(); // Process the command
    }
    // Handle other keys
    else {
        static const char scancode_to_ascii[128] = {
            0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
            '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
            0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
            0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
            '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
            0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
        };

        // Convert scancode to ASCII and add to buffer
        if (key.scancode < 128) {
            char ascii_char = scancode_to_ascii[key.scancode];
            if (ascii_char != 0 && input_buffer_index < sizeof(input_buffer) - 1) {
                input_buffer[input_buffer_index++] = ascii_char;
                printf("%c", ascii_char); // Echo the character to the terminal
            }
        }
    }
}

void main_process() {
    lprintf(logging_level::OK, "Scheduler control given; main process started.\n");
     // Add the shell input handler to the keyboard interrupt system
    ps2_keyboard_add_handler(handle_shell_input);
    init_ps2_keyboard();

    printf("Welcome to the Aquanite shell!\n");
    printf("aqua |> ");
    while (true) {
        // Main loop does nothing; input is interrupt-driven
    }
}

/// --------------------------------- ///