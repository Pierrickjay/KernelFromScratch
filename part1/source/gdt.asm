section .rodata
gdt_start:
    dq 0x0                    ; NULL segment
    dq 0x00CF9A000000FFFF     ; Kernel code segment (0x08)
    dq 0x00CF92000000FFFF     ; Kernel data segment (0x10)

gdt_descriptor:
    dw gdt_descriptor - gdt_start - 1
    dd gdt_start
