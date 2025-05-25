#include "keyboard_interrupts.h"
#include "io.h"
#include "print_manager.h"

t_char_stacked_queue keyboard_queue;

void on_keyboard_press(unsigned char scancode)
{
	if (queue_is_full(&keyboard_queue)) {
		return;
	}
	queue_push(&keyboard_queue, scancode);
}

void on_keyboard_release(unsigned char scancode)
{
	if (queue_is_full(&keyboard_queue)) {
		return;
	}
	queue_push(&keyboard_queue, scancode);
}

void keyboard_handler_c(void)
{
	unsigned char scancode;

	scancode = inb(KEYBOARD_PORT);

	if (scancode < 0x80) {
		on_keyboard_press(scancode);
	}
	else {
		on_keyboard_release(scancode);
	}

	// Signale au PIC que l'interruption est traitée
	outb(0x20, 0x20);
}
