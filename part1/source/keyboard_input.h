#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include "char_stacked_queue.h"
#include "types.h"

#define INPUT_MODE_DISABLED 0
#define INPUT_MODE_NORMAL 1

void set_input_mode(u8 mode);
char get_input_mode(void);

void handle_keyboard_inputs(t_char_stacked_queue *queue);

#endif
