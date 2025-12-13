#ifndef KEYBOARD_INTERRUPTS_H
#define KEYBOARD_INTERRUPTS_H

#include "char_stacked_queue.h"

#define KEYBOARD_PORT 0x60

extern t_char_stacked_queue keyboard_queue;

void keyboard_handler(void);   // ASM wrapper (from interrupt_handlers.asm)
void keyboard_handler_c(void); // C handler callback

#endif
