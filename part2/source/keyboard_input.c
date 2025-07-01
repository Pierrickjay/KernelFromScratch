#include "keyboard_input.h"
#include "keyboard_map.h"
#include "print_manager.h"
#include "mini_minishell.h"

static u8 special_keys		  = 0;
static u8 left_shift_pressed  = 0;
static u8 right_shift_pressed = 0;
static u8 input_mode		  = INPUT_MODE_DISABLED;

static char writeds_char[80] = {0}; // buffer for writeds command

void handle_keyboard_arrow(unsigned char scancode)
{
	switch (scancode) {
		case LEFT_ARROW:
			if (input_mode == INPUT_MODE_NORMAL && screen_context.desktops[screen_context.desktop_index].cursor.x > 0)
				decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
			else if (input_mode == INPUT_MODE_MINISHELL && screen_context.desktops[screen_context.desktop_index].cursor.x > 1)
				decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		case RIGHT_ARROW:
			increment_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		case UP_ARROW:
			if (input_mode == INPUT_MODE_NORMAL)
				set_cursor_on_upper_line(&screen_context.desktops[screen_context.desktop_index].cursor);
			break;
		case DOWN_ARROW:
			if (input_mode == INPUT_MODE_NORMAL)
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
		case BACKSPACE:
			if (screen_context.desktops[screen_context.desktop_index].cursor.x > 0 && input_mode == INPUT_MODE_NORMAL ||
				(screen_context.desktops[screen_context.desktop_index].cursor.x > 1 && input_mode == INPUT_MODE_MINISHELL)) 
			{
				if (input_mode == INPUT_MODE_MINISHELL)
					writeds_char[screen_context.desktops[screen_context.desktop_index].cursor.x - 2] = 0; // Clear the last character in the buffer -2 bc cursor is at pos + 1 and  +1 for ">" 
				decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
				kfs_clear_cursor_cell(&screen_context);
			}
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
			if (c == '\\')
			{
				launch_mini_minishell(); // Initialize the mini minishell
				input_mode = INPUT_MODE_MINISHELL; // Switch to mini minishell mode when pressing \ key
				kfs_write_char(&screen_context, '>');
			}
			else if (c) {
				kfs_write_char(&screen_context, c);
				kfs_clear_cursor_cell(&screen_context);
			}
		}

		else if (input_mode == INPUT_MODE_MINISHELL)
		{
			if (handle_special_key(scancode)) {
				return;
			}

			char c = scancode_to_char(scancode, special_keys);
			if (c) {
				if (c == '\n') {
					writeds_char[80] = c; // Store the newline character
					writeds_char[screen_context.desktops[screen_context.desktop_index].cursor.x] = 0; // Null-terminate the string
					handle_input(writeds_char); // Process the input
					for (int i = 0; i < 80; i++) {
						writeds_char[i] = 0; // Clear the buffer
					}
					if (get_input_mode() == INPUT_MODE_MINISHELL) {
						kfs_write_char(&screen_context, '>'); // Print the prompt again
					}
				} else {
					writeds_char[screen_context.desktops[screen_context.desktop_index].cursor.x - 1] = c; // -1 is bc cursor is at the next position bc of "> "" 
					kfs_write_char(&screen_context, c);
				}
				kfs_clear_cursor_cell(&screen_context);
			}
		}
		else {
			print_k(KERN_ERR "Unknown input mode: %d\n", input_mode);
		}
	}
}

void set_input_mode(u8 mode)
{
	if (mode == INPUT_MODE_NORMAL || mode == INPUT_MODE_MINISHELL) {
		kfs_clear_cursor_cell(&screen_context);
	}
	input_mode = mode;
}

char get_input_mode(void)
{
	return input_mode;
}
