#include <uacpi/kernel_api.h>
#include <limine.h>
#include <idt/isr.h>
uacpi_status uacpi_kernel_raw_memory_read(
    uacpi_phys_addr address, uacpi_u8 byte_width, uacpi_u64 *out_value
)
{
    if (byte_width == 1)
    {
        *out_value = *(uacpi_u8 *)address;
    }
    else if (byte_width == 2)
    {
        *out_value = *(uacpi_u16 *)address;
    }
    else if (byte_width == 4)
    {
        *out_value = *(uacpi_u32 *)address;
    }
    else if (byte_width == 8)
    {
        *out_value = *(uacpi_u64 *)address;
    }
    else
    {
        return UACPI_STATUS_INVALID_ARGUMENT;
    }

    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_raw_memory_write(
    uacpi_phys_addr address, uacpi_u8 byte_width, uacpi_u64 in_value
)
{
    if (byte_width == 1)
    {
        *(uacpi_u8 *)address = in_value;
    }
    else if (byte_width == 2)
    {
        *(uacpi_u16 *)address = in_value;
    }
    else if (byte_width == 4)
    {
        *(uacpi_u32 *)address = in_value;
    }
    else if (byte_width == 8)
    {
        *(uacpi_u64 *)address = in_value;
    }
    else
    {
        return UACPI_STATUS_INVALID_ARGUMENT;
    }

    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_raw_io_read(
    uacpi_io_addr address, uacpi_u8 byte_width, uacpi_u64 *out_value
)
{
    if (byte_width == 1)
    {
        *out_value = inb(address);
    }
    else if (byte_width == 2)
    {
        *out_value = inw(address);
    }
    else if (byte_width == 4)
    {
        *out_value = inl(address);
    }
    else
    {
        return UACPI_STATUS_INVALID_ARGUMENT;
    }

    return UACPI_STATUS_OK;
}
uacpi_status uacpi_kernel_raw_io_write(
    uacpi_io_addr address, uacpi_u8 byte_width, uacpi_u64 in_value
)
{
    if (byte_width == 1)
    {
        outb(address, in_value);
    }
    else if (byte_width == 2)
    {
        outw(address, in_value);
    }
    else if (byte_width == 4)
    {
        outl(address, in_value);
    }
    else
    {
        return UACPI_STATUS_INVALID_ARGUMENT;
    }

    return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len)
{
    unsigned long long vmmmemo = allocate_virtual_memory_kernel(len);
    virtual_map(addr, vmmmemo, true, false, true, false, false, false, 0, true);
    return (void *)vmmmemo;
}
void uacpi_kernel_unmap(void *addr, uacpi_size len)
{
    // unmap
    virtual_unmap((uint64_t)addr);
}

void *uacpi_kernel_alloc(uacpi_size size)
{
    return dlmalloc(size);
}

/*
 * Allocate a block of memory of 'count' * 'size' bytes.
 * The returned memory block is expected to be zero-filled.
 */
void *uacpi_kernel_calloc(uacpi_size count, uacpi_size size)
{
    return dlcalloc(count, size);
}
void uacpi_kernel_free(void *mem)
{
    dlfree(mem);
}
void uacpi_kernel_log(uacpi_log_level lvl, const uacpi_char* lout)
{
    if (lvl == UACPI_LOG_INFO)
    {
        clprintf(1, lout);
    }
    else if (lvl == UACPI_LOG_WARN)
    {
        clprintf(2, lout);
    }
    else if (lvl == UACPI_LOG_ERROR)
    {
        clprintf(3, lout);
    }
    else if (lvl == UACPI_LOG_DEBUG)
    {
        clprintf(4, lout);
    }
    else if (lvl == UACPI_LOG_TRACE)
    {
        clprintf(4, lout);
    }
}
uacpi_u64 uacpi_kernel_get_ticks(void)
{
    return 46644; // STUB
}

/*
 * Spin for N microseconds.
 */
void uacpi_kernel_stall(uacpi_u16 usec)
{
    //while and NOP insruction
    for (int i = 0; i < usec; i++)
    {
        __asm__ __volatile__("nop");
    }
}

/*
 * Sleep for N milliseconds.
 */
void uacpi_kernel_sleep(uacpi_u64 msec)
{
    int i = 0;
    while (i < msec)
    {
        uacpi_kernel_stall(1000);
        i++;
    }
}

/*
 * Create/free an opaque non-recursive kernel mutex object.
 */
uacpi_handle uacpi_kernel_create_mutex(void)
{
    return (void*)1; // STUB
}
void uacpi_kernel_free_mutex(uacpi_handle)
{
    return; // STUB
}

/*
 * Create/free an opaque kernel (semaphore-like) event object.
 */
uacpi_handle uacpi_kernel_create_event(void)
{
    return (void*)1; // STUB
}
void uacpi_kernel_free_event(uacpi_handle)
{
    return; // STUB
}

uacpi_thread_id uacpi_kernel_get_thread_id(void) { return (void*)1; };

uacpi_bool uacpi_kernel_acquire_mutex(uacpi_handle, uacpi_u16)
{
    return UACPI_TRUE; // STUB
}
void uacpi_kernel_release_mutex(uacpi_handle)
{
    return; // STUB
}

uacpi_bool uacpi_kernel_wait_for_event(uacpi_handle, uacpi_u16)
{
    return UACPI_TRUE; // STUB
}

void uacpi_kernel_signal_event(uacpi_handle)
{
    return; // STUB
}

/*
 * Reset the event counter to 0.
 */
void uacpi_kernel_reset_event(uacpi_handle)
{
    return; // STUB
}

/*
 * Handle a firmware request.
 *
 * Currently either a Breakpoint or Fatal operators.
 */
uacpi_status uacpi_kernel_handle_firmware_request(uacpi_firmware_request*)
{
    return UACPI_STATUS_OK; // STUB
}
int allowintsforuacpi = 0;

uacpi_handle uacpi_kernel_create_spinlock(void)
{
    return (void*)1; // STUB
}
void uacpi_kernel_free_spinlock(uacpi_handle lock)
{
    return; // STUB
}

uacpi_cpu_flags uacpi_kernel_lock_spinlock(uacpi_handle lock)
{
    return 0x1DC; // IDC
}
void uacpi_kernel_unlock_spinlock(uacpi_handle lock, uacpi_cpu_flags)
{
    return; // STUB
}

uacpi_status uacpi_kernel_uninstall_interrupt_handler(
    uacpi_interrupt_handler, uacpi_handle irq_handle
) {return UACPI_STATUS_UNIMPLEMENTED;}
void* lim_resp_rs;
void set_uacpi_rsdp(void* rsdp)
{
    lim_resp_rs = rsdp;
}
uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rdsp_address)
{
    *out_rdsp_address = (uint64_t)lim_resp_rs;
    return UACPI_STATUS_OK;
}

uacpi_status uacpi_kernel_pci_read(
    uacpi_pci_address *address, uacpi_size offset,
    uacpi_u8 byte_width, uacpi_u64 *value
)
{
    return UACPI_STATUS_UNIMPLEMENTED;
}
uacpi_status uacpi_kernel_pci_write(
    uacpi_pci_address *address, uacpi_size offset,
    uacpi_u8 byte_width, uacpi_u64 value
)
{
    return UACPI_STATUS_UNIMPLEMENTED;
}
struct io_range {
  uacpi_io_addr base;
  uacpi_size len;
};
uacpi_status uacpi_kernel_io_map(
    uacpi_io_addr base, uacpi_size len, uacpi_handle *out_handle
)
{
    struct io_range *range = (struct io_range *)malloc(sizeof(struct io_range));
    range->base = base;
    range->len = len;
    *out_handle = (uacpi_handle)range;
    return UACPI_STATUS_OK;
}
void uacpi_kernel_io_unmap(uacpi_handle handle)
{
    free((struct io_range *)handle);
}


uacpi_status uacpi_kernel_schedule_work(
    uacpi_work_type, uacpi_work_handler, uacpi_handle ctx
)
{
    return UACPI_STATUS_UNIMPLEMENTED;
}

/*
 * Blocks until all scheduled work is complete and the work queue becomes empty.
 */
uacpi_status uacpi_kernel_wait_for_work_completion(void)
{
    return UACPI_STATUS_UNIMPLEMENTED;
}

uacpi_status uacpi_kernel_io_read(uacpi_handle hnd, uacpi_size offset,
                                  uacpi_u8 byte_width, uacpi_u64 *value) {
  struct io_range *rng = (struct io_range *)hnd;
  if (offset >= rng->len) {
    return UACPI_STATUS_INVALID_ARGUMENT;
  }
  return uacpi_kernel_raw_io_read(rng->base + offset, byte_width, value);
}
uacpi_status uacpi_kernel_io_write(uacpi_handle hnd, uacpi_size offset,
                                   uacpi_u8 byte_width, uacpi_u64 value) {
  struct io_range *rng = (struct io_range *)hnd;
  if (offset >= rng->len) {
    return UACPI_STATUS_INVALID_ARGUMENT;
  }
  return uacpi_kernel_raw_io_write(rng->base + offset, byte_width, value);
}
unsigned int (*calller)(void*);
void acpicallint(struct registers_t* r)
{
    calller((void*)0);
}
uacpi_status uacpi_kernel_install_interrupt_handler(
    uacpi_u32 irq, uacpi_interrupt_handler handler, uacpi_handle ctx,
    uacpi_handle *out_irq_handle
)
{
    //get irq it wants to set using install_isr_at
    install_isr_at(irq, acpicallint);
    calller = handler;
    return UACPI_STATUS_OK;
}