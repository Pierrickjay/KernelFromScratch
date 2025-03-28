
global gdt_load
extern gdt_descriptor

section .text
gdt_load:
    ; Check if gdt_ptr is valid (non-zero)
    mov eax, [gdt_descriptor]
    test eax, eax
    jz .error

    ; Disable interrupts
    cli

    ; Load the GDT
    lgdt [gdt_descriptor]

    ; Reload segment registers
    mov ax, 0x10    ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Far jump to reload code segment
    jmp 0x08:.reload_cs

.reload_cs:
    ; Re-enable interrupts
    sti
    ret

.error:
    ; If gdt_ptr is invalid, you might want to:
    ; 1. Halt the CPU
    ; 2. Send an error code
    ; 3. Trigger a breakpoint
    cli
    hlt
