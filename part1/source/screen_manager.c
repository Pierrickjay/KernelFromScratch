
#include "screen_manager.h"
#include "memory_manager.h"

t_screen_context screen_context;

void init_desktop(t_desktop *desktop)
{
	init_position(&desktop->cursor);
	ft_memset(desktop->cells, 0, SCREEN_BUFFER_SIZE);
}

void init_screen_context(t_screen_context *ctx)
{
	ctx->vga_buffer	   = (unsigned char *)VGA_ADDRESS;
	ctx->desktop_index = 0;
	for (int index = 0; index < DESKTOP_COUNT; index++)
	{
		init_desktop(&ctx->desktops[index]);
	}
}

void set_char_cell(t_screen_context *ctx, const t_position *pos, t_character_cell cell)
{
	if (pos->x >= L_WINDOW || pos->y >= H_WINDOW)
		return; // @TODO printk("ERROR: cell out of bounds\n");

	// Save the character and color in the cell
	ctx->desktops[ctx->desktop_index].cells[pos->x][pos->y] = cell;

	// Display the character on the screen
	unsigned long vga_index = (pos->y * L_WINDOW + pos->x) * 2;

	ctx->vga_buffer[vga_index]	   = cell.character;
	ctx->vga_buffer[vga_index + 1] = cell.color;
}

void set_cursor_on_next_line(t_position *cursor)
{
	cursor->x = 0;
	cursor->y = (cursor->y + 1) % H_WINDOW;
}

void increment_cursor_by(t_desktop *desktop, int nb)
{
	t_position *cursor = &desktop->cursor;

	unsigned int new_x = cursor->x + nb;
	if (new_x >= L_WINDOW)
	{
		set_cursor_on_next_line(cursor);
		cursor->x = new_x % L_WINDOW;
	}
}

void increment_cursor(t_desktop *desktop)
{
	desktop->cursor.x++;
	if (desktop->cursor.x >= L_WINDOW)
	{
		set_cursor_on_next_line(&desktop->cursor);
	}
}

void set_cursor(t_screen_context *ctx, t_position pos)
{
	if (pos.x >= L_WINDOW || pos.y >= H_WINDOW)
		return; // @TODO printk("ERROR: cursor out of bounds\n");
	ctx->desktops[ctx->desktop_index].cursor = pos;
}

void kfs_write_char(t_screen_context *ctx, unsigned char c, unsigned char color)
{
	t_desktop *desktop = &ctx->desktops[ctx->desktop_index];

	switch (c)
	{
		case '\n':
			// @TODO clear rest of line
			set_cursor_on_next_line(&desktop->cursor);
			break;
		case '\t':
			// @TODO clear the space between the cursor and the next tab stop
			increment_cursor_by(desktop, 4); // @TODO go to next tab stop
			break;
		default:
			set_char_cell(ctx, &desktop->cursor, (t_character_cell){color, c});
			increment_cursor(desktop);
			break;
	}
}

void clear_screen(t_screen_context *ctx)
{
	set_cursor(ctx, (t_position){0, 0});
	for (int index = 0; index < SCREEN_CELLS_SIZE; index++)
	{
		kfs_write_char(ctx, ' ', WHITE);
	}
}
