#include "keyboard_map.h"

// Table de conversion scancode -> caractère (sans shift)
const char scancode_to_ascii[] = {0,   0,	'1',  '2',	'3',  '4', '5', '6',  '7', '8', '9', '0',
								  '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
								  'o', 'p', '[',  ']',	'\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
								  'j', 'k', 'l',  ';',	'\'', '`', 0,	'\\', 'z', 'x', 'c', 'v',
								  'b', 'n', 'm',  ',',	'.',  '/', 0,	'*',  0,   ' '};

// Table de conversion scancode -> caractère (avec shift)
const char scancode_to_ascii_shift[] = {
	0,	  0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',	'\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
	'A',  'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,	  '|',	'Z',
	'X',  'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,	 ' '};

char scancode_to_char(u8 scancode, u8 special_keys)
{
	if (scancode >= sizeof(scancode_to_ascii))
		return 0;

	if (special_keys & SHIFT_PRESSED || special_keys & CAPS_LOCK)
		return scancode_to_ascii_shift[scancode];

	return scancode_to_ascii[scancode];
}
