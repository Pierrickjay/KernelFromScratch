#include "keyboard_text_mode_map.h"
#include "keyboard_input.h"
#include "print_manager.h"


#define KEY_HANDLED 0

static u8 special_keys		  = 0;
static u8 left_shift_pressed  = 0;
static u8 right_shift_pressed = 0;

static char writeds_char[80] = {0}; // buffer for writeds command

static u16 handle_left_arrow(void)
{
	int input_mode = get_input_mode();
	if (input_mode == INPUT_MODE_NORMAL && screen_context.desktops[screen_context.desktop_index].cursor.x > 0)
		decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
	else if (input_mode == INPUT_MODE_MINISHELL && screen_context.desktops[screen_context.desktop_index].cursor.x > 1)
		decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
	return KEY_HANDLED;
}

static u16 handle_right_arrow(void)
{
	increment_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
	return KEY_HANDLED;
}

static u16 handle_up_arrow(void)
{
	if (get_input_mode() == INPUT_MODE_NORMAL)
		set_cursor_on_upper_line(&screen_context.desktops[screen_context.desktop_index].cursor);
	return KEY_HANDLED;
}

static u16 handle_down_arrow(void)
{
	if (get_input_mode() == INPUT_MODE_NORMAL)
		set_cursor_on_next_line(&screen_context.desktops[screen_context.desktop_index].cursor);
	return KEY_HANDLED;
}

static u16 handle_left_shift(void)
{
	left_shift_pressed = !left_shift_pressed;
	if (left_shift_pressed) {
		special_keys |= SHIFT_PRESSED;
	}
	else {
		if (!right_shift_pressed) {
			special_keys &= ~SHIFT_PRESSED;
		}
	}
	return KEY_HANDLED;
}

static u16 handle_right_shift(void)
{
	right_shift_pressed = !right_shift_pressed;
	if (right_shift_pressed) {
		special_keys |= SHIFT_PRESSED;
	}
	else {
		if (!left_shift_pressed) {
			special_keys &= ~SHIFT_PRESSED;
		}
	}
	return KEY_HANDLED;
}

static u16 handle_caps_lock(void)
{
	special_keys ^= CAPS_LOCK;
	return KEY_HANDLED;
}

static u16 handle_f1(void)
{
	change_desktop(&screen_context, 0);
	return KEY_HANDLED;
}

static u16 handle_f2(void)
{
	change_desktop(&screen_context, 1);
	return KEY_HANDLED;
}

static u16 handle_f3(void)
{
	change_desktop(&screen_context, 2);
	return KEY_HANDLED;
}

static u16 handle_f4(void)
{
	change_desktop(&screen_context, 3);
	return KEY_HANDLED;
}

static u16 handle_backspace(void)
{
	int input_mode = get_input_mode();
	if (screen_context.desktops[screen_context.desktop_index].cursor.x > 0 && input_mode == INPUT_MODE_NORMAL ||
				(screen_context.desktops[screen_context.desktop_index].cursor.x > 1 && input_mode == INPUT_MODE_MINISHELL)) 
	{
		if (input_mode == INPUT_MODE_MINISHELL)
			writeds_char[screen_context.desktops[screen_context.desktop_index].cursor.x - 2] = 0; // Clear the last character in the buffer -2 bc cursor is at pos + 1 and  +1 for ">" 
		decrement_cursor(&screen_context.desktops[screen_context.desktop_index].cursor);
		kfs_clear_cursor_cell(&screen_context);
	}
	return KEY_HANDLED;
}

typedef u16 (*key_handler_t)(void);

static const key_handler_t custom_keys_map[KEYBOARD_INDEXES_LENGTH] = {
	[LEFT_ARROW_PRESSED_INDEX]	 = handle_left_arrow,
	[RIGHT_ARROW_PRESSED_INDEX]	 = handle_right_arrow,
	[UP_ARROW_PRESSED_INDEX]	 = handle_up_arrow,
	[DOWN_ARROW_PRESSED_INDEX]	 = handle_down_arrow,
	[CURSOR_LEFT_PRESSED_INDEX]	 = handle_left_arrow,
	[CURSOR_RIGHT_PRESSED_INDEX] = handle_right_arrow,
	[CURSOR_UP_PRESSED_INDEX]	 = handle_up_arrow,
	[CURSOR_DOWN_PRESSED_INDEX]	 = handle_down_arrow,

	[LSHIFT_PRESSED_INDEX]	 = handle_left_shift,
	[RSHIFT_PRESSED_INDEX]	 = handle_right_shift,
	[LSHIFT_RELEASED_INDEX]	 = handle_left_shift,
	[RSHIFT_RELEASED_INDEX]	 = handle_right_shift,
	[CAPSLOCK_PRESSED_INDEX] = handle_caps_lock,

	[F1_PRESSED_INDEX] = handle_f1,
	[F2_PRESSED_INDEX] = handle_f2,
	[F3_PRESSED_INDEX] = handle_f3,
	[F4_PRESSED_INDEX] = handle_f4,

	[BACKSPACE_PRESSED_INDEX] = handle_backspace,
};

const char keyboard_index_to_ascii[] = {
	0,	  0,   '1', '2', '3', '4', '5', '6', '7', '8', '9',	 '0', '-', '=',	 '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',	 '[', ']', '\n', 0,
	'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z',
	'x',  'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',	 0,	  ' '};

const char keyboard_index_to_ascii_shift[] = {
	0,	  0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',	'\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
	'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,	  '|',	'Z',
	'X',  'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,	 ' '};

/**
 * Handles special keys or returns keyboard_index for character output.
 * @param keyboard_index The keyboard keyboard_index to process
 * @return KEY_HANDLED if special key was processed and nothing to write, keyboard_index for
 * character output otherwise
 */
static u16 handle_custom_keys(u16 keyboard_index)
{
	key_handler_t handler = custom_keys_map[keyboard_index];
	if (handler)
		return handler();
	return keyboard_index;
}

char convert_keyboard_index_to_ascii(u16 keyboard_index)
{
	if (keyboard_index >= sizeof(keyboard_index_to_ascii))
		return 0;

	if (special_keys & (SHIFT_PRESSED | CAPS_LOCK))
		return keyboard_index_to_ascii_shift[keyboard_index];

	return keyboard_index_to_ascii[keyboard_index];
}

void handle_ascii_char(u16 keyboard_index)
{
	int input_mode = get_input_mode();
	char c = convert_keyboard_index_to_ascii(keyboard_index);
	if (c) {
		if (c == '\\' && input_mode == INPUT_MODE_NORMAL)
		{
			launch_mini_minishell();
			set_input_mode(INPUT_MODE_MINISHELL);
			return;
		}
		if (input_mode == INPUT_MODE_MINISHELL)
		{
			if (c == '\n')
			{
				writeds_char[screen_context.desktops[screen_context.desktop_index].cursor.x - 1] = 0; // null terminated the last character
				handle_input(writeds_char);
				for (int i = 0; i < 80; i++)
					writeds_char[i] = 0; // Clear the buffer
				if (get_input_mode() == INPUT_MODE_MINISHELL) {
					kfs_write_char(&screen_context, '>'); // need to check again bc can change in handle_input
					return;
				}
			}
			else
				writeds_char[screen_context.desktops[screen_context.desktop_index].cursor.x - 1] = c;
		}
		kfs_write_char(&screen_context, c);
		kfs_clear_cursor_cell(&screen_context);
	}
}

void handle_keyboard_text_mode(u16 keyboard_index)
{
	u16 result = handle_custom_keys(keyboard_index);

	if (result == KEY_HANDLED)
		return;
	
	handle_ascii_char(result);
}
