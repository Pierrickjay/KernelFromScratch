#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include "types.h"
#include "mini_minishell.h"

#define SHIFT_PRESSED 0x1
#define CAPS_LOCK 0x2
#define CTRL_PRESSED 0x4
#define WINDOWS_PRESSED 0x8
#define ALT_PRESSED 0x10
#define RIGHT_ALT_PRESSED 0x20

void handle_keyboard_text_mode(u16 scancode);

#endif
