#include "keyboard_interrupts.h"
#include "io.h"
#include "print_manager.h"

t_char_stacked_queue keyboard_queue;

void on_keyboard_interrupt(unsigned char scancode)
{
	if (queue_is_full(&keyboard_queue)) {
		return;
	}
	queue_push(&keyboard_queue, scancode);
}

void keyboard_handler_c(void)
{
	on_keyboard_interrupt(inb(KEYBOARD_PORT));

	// Signale au PIC que l'interruption est traitée
	outb(0x20, 0x20);
}
