bits 32

section .multiboot               ;according to multiboot spec
        dd 0x1BADB002            ; Magic number (permet au BIOS de reconnaître le kernel)
        dd 0x0                   ; Flags
        dd - (0x1BADB002 + 0x0)  ; Checksum (pour vérifier l'intégrité)

section .text
global start
extern main                      ;defined in the C file

start:
        cli                      ;block interrupts
        lgdt [gdt_descriptor]

        mov ax, 0x10        ; Segment DATA kernel
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        jmp 0x08:protected_mode_entry

protected_mode_entry:
        mov esp, stack_space     ;set stack pointer
        call main
.halt:
        hlt                   ; Halt CPU (if main returns)
        jmp .halt

section .bss
stack_bottom:
        resb 8192              ; 8 Ko de pile
stack_space:

%include "source/gdt.asm"
