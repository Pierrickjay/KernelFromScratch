#ifndef CURSOR_H
#define CURSOR_H

#include "types.h"
#include "utils.h"
#include "keyboard_input.h"

#define CURSOR_PORT_CMD 0x3D4
#define CURSOR_PORT_DATA 0x3D5

#define CURSOR_START_REG 0x0A
#define CURSOR_END_REG 0x0B
#define CURSOR_HIGH_REG 14
#define CURSOR_LOW_REG 15

#define t_cursor t_position

void init_cursor(t_cursor *cursor);

void set_cursor_on_next_line(t_cursor *cursor);
void set_cursor_on_upper_line(t_cursor *cursor);
void increment_cursor_by(t_cursor *cursor, int nb);
void decrement_cursor_by(t_cursor *cursor, int nb);
void increment_cursor(t_cursor *cursor);
void decrement_cursor(t_cursor *cursor);
void set_cursor(t_cursor *cursor, t_position pos);


#endif
