#include "keyboard_input.h"
#include "keyboard_map.h"
#include "print_manager.h"

static u8 special_keys		  = 0;
static u8 left_shift_pressed  = 0;
static u8 right_shift_pressed = 0;
static u8 input_mode		  = INPUT_MODE_DISABLED;

void handle_keyboard_arrow(unsigned char scancode)
{
	switch (scancode) {
		case LEFT_ARROW:
			decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		case RIGHT_ARROW:
			increment_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		case UP_ARROW:
			set_cursor_on_upper_line(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		case DOWN_ARROW:
			set_cursor_on_next_line(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		default:
			print_k(KERN_ERR "Unknown arrow key scancode: %d\n", scancode);
			break;
	}
}

static int handle_special_key(unsigned char scancode)
{
	if (scancode == LEFT_ARROW || scancode == RIGHT_ARROW || scancode == UP_ARROW || scancode == DOWN_ARROW) {
		handle_keyboard_arrow(scancode);
		return 1;
	}
	
	switch (scancode) {
		case SCANCODE_LSHIFT:
			left_shift_pressed = 1;
			special_keys |= SHIFT_PRESSED;
			return 1;
		case SCANCODE_RSHIFT:
			right_shift_pressed = 1;
			special_keys |= SHIFT_PRESSED;
			return 1;
		case (SCANCODE_LSHIFT | 0x80):
			left_shift_pressed = 0;
			if (!right_shift_pressed)
				special_keys &= ~SHIFT_PRESSED;
			return 1;
		case (SCANCODE_RSHIFT | 0x80):
			right_shift_pressed = 0;
			if (!left_shift_pressed)
				special_keys &= ~SHIFT_PRESSED;
			return 1;
		case SCANCODE_CAPS_LOCK:
			special_keys ^= CAPS_LOCK;
			return 1;
		default:
			return 0;
	}
}

void handle_keyboard_inputs(t_char_stacked_queue *queue)
{
	while (!queue_is_empty(queue)) {
		unsigned char scancode = queue_pop(queue);
		if (input_mode == INPUT_MODE_DISABLED) {
			continue;
		}

		if (input_mode == INPUT_MODE_NORMAL) {
			if (handle_special_key(scancode)) {
				return;
			}

			char c = scancode_to_char(scancode, special_keys);
			if (c) {
				kfs_write_char(&screen_context, c);
				kfs_clear_cursor_cell(&screen_context);
			}
		}
	}
}

void set_input_mode(u8 mode)
{
	if (mode == INPUT_MODE_NORMAL) {
		kfs_clear_cursor_cell(&screen_context);
	}
	input_mode = mode;
}

char get_input_mode(void)
{
	return input_mode;
}
