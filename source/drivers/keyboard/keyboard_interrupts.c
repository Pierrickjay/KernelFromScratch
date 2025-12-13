#include "keyboard_interrupts.h"
#include "io.h"

t_char_stacked_queue keyboard_queue;

void on_keyboard_interrupt(unsigned char scancode)
{
	// Touche normale : met dans la queue
	if (queue_is_full(&keyboard_queue)) {
		return;
	}
	queue_push(&keyboard_queue, scancode);
}

void keyboard_handler_c(void)
{
	on_keyboard_interrupt(inb(KEYBOARD_PORT));

	// Signale au PIC que l'interruption est traitee
	outb(0x20, 0x20);
}
