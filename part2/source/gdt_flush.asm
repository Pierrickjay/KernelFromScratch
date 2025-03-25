global gdt_flush
extern gdt_ptr

gdt_flush:
    cli                 ; Disable interrupts
    lgdt [gdt_ptr]      ; Load the GDT pointer
    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax          ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ax, 0x18        ; 0x18 is the offset in the GDT to our kernel stack
    mov ss, ax
    jmp 0x08:.flush     ; 0x08 is the offset to our code segment: far jump
.flush:
    ret
