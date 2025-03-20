bits 32

section .multiboot               ;according to multiboot spec
        dd 0x1BADB002            ; Magic number (permet au BIOS de reconnaître ton kernel)
        dd 0x0                   ; Flags (options de démarrage)
        dd - (0x1BADB002 + 0x0)  ; Checksum (pour vérifier l'intégrité)

section .text
global start
extern main                      ;defined in the C file
start:
        cli                      ;block interrupts
        mov esp, stack_space     ;set stack pointer
        call main
        hlt                      ;halt the CPU

section .bss
resb 8192                        ;8KB for stack
stack_space:
