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
/// --------------------------------- ///

/// ---------- GLOBAL FUNCS ---------- ///
extern void kernel_main() {
    limine_framebuffer* framebuffer;
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
    );
    flanterm_write(ft_ctx, "FLANTERM CONTEXT MADE\n", 23);
    flanterm_set_instance(ft_ctx);
    printf("\"printf\" set up.\n");
    GDT_init();
    printf("GDT initialized.\n");
    set_heap_start(hhdm_request.response->offset);
    char* test = (char*)dlmalloc(6);
    test[0] = 'H';
    test[1] = 'e';
    test[2] = 'l';
    test[3] = 'l';
    test[4] = 'o';
    test[5] = '\0';
    printf("Heap test: %u\n", test);
    char* test2 = (char*)dlmalloc(7);
    test2[0] = 'W';
    test2[1] = 'o';
    test2[2] = 'r';
    test2[3] = 'l';
    test2[4] = 'd';
    test2[5] = '!';
    test2[6] = '\0';
    printf("Heap test 2: %u\n", test2);
    //free the first and second test
    dlfree(test);
    //malloc a new test
    char* test3 = (char*)dlmalloc(6);
    test3[0] = 'H';
    test3[1] = 'e';
    test3[2] = 'l';
    test3[3] = 'l';
    test3[4] = 'o';

    test3[5] = '\0';
    printf("Heap test 3: %u\n", test3);
    while (true)
    {
    }
    halt();
}
/// --------------------------------- ///