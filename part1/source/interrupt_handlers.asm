global keyboard_handler
extern keyboard_handler_c

keyboard_handler:
    pusha
    call keyboard_handler_c
    popa
    iret
