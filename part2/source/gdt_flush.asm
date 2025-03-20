global _gdt_flush ; make the function available to other files

extern _gp ; _gp is in an other file

_gdt_flush:
    lgdt [_gp] ; load the GDT
    mov ax, 0x10 ; 0x10 is the offset in the GDT of the kernel code segment
    mov ds, ax ; load the data segment registers
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax ; load the stack segment register
    jmp 0x08:flush ; 0x08 is the offset in the GDT of the kernel data segment
flush:
    ret ; return to the caller
