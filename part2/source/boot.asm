bits 32

section .multiboot
    dd 0x1BADB002            ; Magic number
    dd 0x00000001            ; Flags (memory info)
    dd -(0x1BADB002 + 0x00000001)  ; Checksum

section .text
global start
extern main
extern gdt_load

extern gdt_install

start:
    ; Ensure stack is set up before any function calls
    mov esp, stack_space

    call gdt_install
    ; Load the GDT
    call gdt_load

    ; Call main kernel function
    call main

.hang:
    cli
    hlt
    jmp .hang

section .bss
    resb 8192            ; 8 KB stack
stack_space:
