#ifndef KEYBOARD_INTERRUPTS_H
#define KEYBOARD_INTERRUPTS_H

#define KEYBOARD_PORT 0x60

void keyboard_handler(void); // from interrupt_handlers.asm

#endif
