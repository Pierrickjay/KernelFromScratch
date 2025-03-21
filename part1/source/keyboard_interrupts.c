#include "keyboard_interrupts.h"
#include "io.h"
#include "print_manager.h"

void on_keyboard_press(unsigned char scancode)
{
	if (scancode)
		print_f("%d ", scancode);
}

void on_keyboard_release(unsigned char scancode)
{
}

void keyboard_handler_c(void)
{
	unsigned char scancode;

	scancode = inb(KEYBOARD_PORT);

	if (scancode < 0x80)
	{
		on_keyboard_press(scancode);
	}
	else
	{
		on_keyboard_release(scancode & 0x7F); // scancode - 0x80
	}

	// Signale au PIC que l'interruption est traitée
	outb(0x20, 0x20);
}
