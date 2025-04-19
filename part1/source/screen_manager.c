
#include "screen_manager.h"
#include "memory_manager.h"
#include "print_manager.h"

t_screen_context screen_context;

void init_desktop(t_desktop *desktop)
{
	init_cursor(&desktop->cursor);
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
	{
		print_k(KERN_ERR "ERROR: cell out of bounds\n");	
		return;
	}

	// Save the character and color in the cell
	ctx->desktops[ctx->desktop_index].cells[pos->x][pos->y] = cell;

	// Display the character on the screen
	unsigned long vga_index = (pos->y * L_WINDOW + pos->x) * 2;

	ctx->vga_buffer[vga_index]	   = cell.character;
	ctx->vga_buffer[vga_index + 1] = cell.color;
}

void kfs_write_colored_char(t_screen_context *ctx, unsigned char c, unsigned char color)
{
	t_desktop *desktop = &ctx->desktops[ctx->desktop_index];
	t_cursor  *cursor  = &desktop->cursor;

	switch (c)
	{
		case '\n':
			// @TODO clear rest of line
			set_cursor_on_next_line(cursor);
			break;
		case '\t':
			// @TODO clear the space between the cursor and the next tab stop
			increment_cursor_by(cursor, 4); // @TODO go to next tab stop
			break;
		default:
			set_char_cell(ctx, cursor, (t_character_cell){color, c});
			increment_cursor(cursor);
			break;
	}
	set_char_cell(ctx, cursor, (t_character_cell){color, ' '});
}

void kfs_write_char(t_screen_context *ctx, unsigned char c)
{
	kfs_write_colored_char(ctx, c, ctx->color);
}

void clear_screen_colored(t_screen_context *ctx, unsigned char color)
{
	set_cursor(&ctx->desktops[ctx->desktop_index].cursor, (t_position){0, 0});
	for (int index = 0; index < SCREEN_CELLS_SIZE; index++)
	{
		kfs_write_colored_char(ctx, ' ', color);
	}
}

void clear_screen(t_screen_context *ctx)
{
	clear_screen_colored(ctx, BLACK);
}

void set_color(t_screen_context *ctx, unsigned char color)
{
	ctx->color = color;
}
