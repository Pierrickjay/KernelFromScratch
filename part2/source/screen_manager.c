#include "screen_manager.h"
#include "memory_manager.h"
#include "print_manager.h"
#include "mini_minishell.h"

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
	ctx->color		   = BG(BLACK) | WHITE;
	for (int index = 0; index < DESKTOP_COUNT; index++) {
		init_desktop(&ctx->desktops[index]);
	}
}

void set_char_cell(t_screen_context *ctx, const t_position *pos, t_character_cell cell)
{
	if (pos->x >= L_SCREEN || pos->y >= H_SCREEN) {
		print_k(KERN_ERR "ERROR: cell out of bounds\n");
		return;
	}

	// Save the character and color in the cell
	ctx->desktops[ctx->desktop_index].cells[pos->x][pos->y] = cell;

	// Display the character on the screen
	unsigned long vga_index = (pos->y * L_SCREEN + pos->x) * 2;

	ctx->vga_buffer[vga_index]	   = cell.character;
	ctx->vga_buffer[vga_index + 1] = cell.color;
}

void kfs_write_colored_char(t_screen_context *ctx, unsigned char c, unsigned char color)
{
	t_desktop *desktop = &ctx->desktops[ctx->desktop_index];
	t_cursor  *cursor  = &desktop->cursor;

	switch (c) {
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
}

void kfs_write_char(t_screen_context *ctx, unsigned char c)
{
	kfs_write_colored_char(ctx, c, ctx->color);
	// print_serial("cursor at pos = (%d, %d)\n",
	// 			 ctx->desktops[ctx->desktop_index].cursor.x,
	// 			 ctx->desktops[ctx->desktop_index].cursor.y);
}

void kfs_clear_cursor_cell(t_screen_context *ctx)
{
	set_char_cell(ctx, &ctx->desktops[ctx->desktop_index].cursor,
				  (t_character_cell){ctx->color, ' '});
}

void clear_screen_colored(t_screen_context *ctx, unsigned char color)
{
	set_cursor(&ctx->desktops[ctx->desktop_index].cursor, (t_position){0, 0});
	for (int index = 0; index < SCREEN_CELLS_SIZE; index++) {
		kfs_write_colored_char(ctx, ' ', color);
	}
}

void clear_screen(t_screen_context *ctx)
{
	clear_screen_colored(ctx, BLACK);
}

unsigned char get_color(t_screen_context *ctx)
{
	return ctx->color;
}

void set_color(t_screen_context *ctx, unsigned char color)
{
	ctx->color = color;
}

unsigned char get_color_value(const char *name) {
    for (size_t i = 0; i < COLOR_COUNT; ++i) {
        if (strcmp(available_colors[i].name, name) == 0) {
            return available_colors[i].value;
        }
    }
	print_k(KERN_ERR "ERROR: color '%s' not found\nSwitching to white", name);
    return WHITE; // Default to white if color not found // maybe change this to an error value
}

void scroll_screen(t_screen_context *ctx)
{
	t_desktop *desktop = &ctx->desktops[ctx->desktop_index];

	// Move all lines up by one
	for (int y = 0; y < H_SCREEN - 1; y++) {
		for (int x = 0; x < L_SCREEN; x++) {
			// Copy the line below to the current line
			desktop->cells[x][y] = desktop->cells[x][y + 1];

			// Update the VGA buffer
			unsigned long vga_index = (y * L_SCREEN + x) * 2;
			ctx->vga_buffer[vga_index] = desktop->cells[x][y].character;
			ctx->vga_buffer[vga_index + 1] = desktop->cells[x][y].color;
		}
	}

	// Clear the last line
	for (int x = 0; x < L_SCREEN; x++) {
		desktop->cells[x][H_SCREEN - 1] = (t_character_cell){ctx->color, ' '};

		// Update the VGA buffer for the last line
		unsigned long vga_index = ((H_SCREEN - 1) * L_SCREEN + x) * 2;
		ctx->vga_buffer[vga_index] = ' ';
		ctx->vga_buffer[vga_index + 1] = ctx->color;
	}
}