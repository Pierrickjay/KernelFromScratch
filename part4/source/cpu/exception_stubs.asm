[BITS 32]

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push %1
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19

extern exception_dispatcher

isr_common_stub:
    pusha ; push EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI

    mov ax, ds
    push eax ; save ds
    mov ax, es
    push eax ; save es
    mov ax, fs
    push eax ; save fs
    mov ax, gs
    push eax ; save gs

    mov ax, 0x10 ; kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp ; push the stack pointer
    call exception_dispatcher

    ; Now we must not be here but... we are, so we need to restore the segment registers
    pop eax
    pop eax
    mov gs, eax
    pop eax
    mov fs, eax
    pop eax
    mov es, eax
    pop eax
    mov ds, eax

    popa
    add esp, 8
    iret
