global keyboard_handler
global timer_handler
extern dispatch_interrupt_handler

; Handler pour le timer (IRQ0)
timer_handler:
    pusha
    push byte 0x20              ; Numero d'interruption (IRQ0 = timer)
    call dispatch_interrupt_handler
    add esp, 4                  ; Nettoie la stack (1 parametre = 4 octets)
    popa
    iret

; Handler pour le clavier (IRQ1)
keyboard_handler:
    pusha
    push byte 0x21              ; Numero d'interruption (IRQ1 = clavier)
    call dispatch_interrupt_handler
    add esp, 4                  ; Nettoie la stack (1 parametre = 4 octets)
    popa
    iret
