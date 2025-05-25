#include "cursor.h"
#include "io.h"
#include "screen_manager.h"
#include "print_manager.h"

void enable_cursor(u8 cursor_start, u8 cursor_end)
{
	outb(CURSOR_PORT_CMD, CURSOR_START_REG);
	outb(CURSOR_PORT_DATA, (inb(CURSOR_PORT_DATA) & 0xC0) | cursor_start);

	outb(CURSOR_PORT_CMD, CURSOR_END_REG);
	outb(CURSOR_PORT_DATA, (inb(CURSOR_PORT_DATA) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	outb(0x3D4, CURSOR_START_REG);
	outb(0x3D5, 0x20);
}

void move_cursor(i16 position)
{
	outb(0x3D4, CURSOR_LOW_REG);
	outb(0x3D5, (u8)(position & 0xFF));
	outb(0x3D4, CURSOR_HIGH_REG);
	outb(0x3D5, (u8)((position >> 8) & 0xFF));
}

void init_cursor(t_cursor *cursor)
{
	cursor->x = 0;
	cursor->y = 0;
	enable_cursor(14, 15);
}

void update_hardware_cursor(t_cursor *cursor)
{
	move_cursor(cursor->y * L_WINDOW + cursor->x);
}

void set_cursor_on_next_line(t_cursor *cursor)
{
	cursor->x = 0;
	cursor->y = (cursor->y + 1) % H_WINDOW;
	update_hardware_cursor(cursor);
}

void increment_cursor_by(t_cursor *cursor, int nb)
{
	unsigned int new_x = cursor->x + nb;
	if (new_x >= L_WINDOW) {
		set_cursor_on_next_line(cursor);
		cursor->x = new_x % L_WINDOW;
	}
	update_hardware_cursor(cursor);
}

void increment_cursor(t_cursor *cursor)
{
	cursor->x++;
	if (cursor->x >= L_WINDOW) {
		set_cursor_on_next_line(cursor);
	}
	update_hardware_cursor(cursor);
}

void set_cursor(t_cursor *cursor, t_position pos)
{
	if (pos.x >= L_WINDOW || pos.y >= H_WINDOW)
	{
		print_k(KERN_ERR "ERROR: cell out of bounds\n");
		return;
	}
	*cursor = pos;
	update_hardware_cursor(cursor);
}
