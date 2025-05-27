#include "keyboard_input.h"
#include "keyboard_map.h"
#include "print_manager.h"

static u8 special_keys		  = 0;
static u8 left_shift_pressed  = 0;
static u8 right_shift_pressed = 0;

void handle_keyboard_input(t_char_stacked_queue *queue)
{
	if (!queue_is_empty(queue)) {
		unsigned char scancode = queue_pop(queue);

		// Gestion des touches spéciales
		if (scancode == SCANCODE_LSHIFT) {
			left_shift_pressed = 1;
			special_keys |= SHIFT_PRESSED;
			return;
		}
		else if (scancode == SCANCODE_RSHIFT) {
			right_shift_pressed = 1;
			special_keys |= SHIFT_PRESSED;
			return;
		}
		else if (scancode == (SCANCODE_LSHIFT | 0x80)) {
			left_shift_pressed = 0;
			if (!right_shift_pressed)
				special_keys &= ~SHIFT_PRESSED;
			return;
		}
		else if (scancode == (SCANCODE_RSHIFT | 0x80)) {
			right_shift_pressed = 0;
			if (!left_shift_pressed)
				special_keys &= ~SHIFT_PRESSED;
			return;
		}
		else if (scancode == SCANCODE_CAPS_LOCK) {
			special_keys ^= CAPS_LOCK;
			return;
		}

		// Conversion du scancode en caractère
		char c = scancode_to_char(scancode, special_keys);
		if (c) {
			kfs_write_char(&screen_context, c);
		}
	}
}
