section .text
global GDT_flush
; C: void GDT_flush(uint64_t gdt_ptr)
GDT_flush:
    lgdt [rdi]
    ret