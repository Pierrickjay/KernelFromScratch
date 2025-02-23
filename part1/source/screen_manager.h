#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "utils.h"

#define VGA_ADDRESS 0xB8000

// Colors
#define BLACK 0
#define BLUE 0b0001
#define GREEN 0b0010
#define RED 0b0100
#define INTENSIVE 0b1000

#define CYAN BLUE | GREEN
#define YELLOW RED | GREEN
#define MAGENTA BLUE | RED
#define WHITE BLUE | GREEN | RED

#define BG(color) (color << 4)

// Window size (determines in boot.asm)
#define H_WINDOW 25
#define L_WINDOW 80

#define SCREEN_BUFFER_SIZE (H_WINDOW * L_WINDOW * 2)

typedef struct s_character_cell
{
	unsigned char color;	 // private
	unsigned char character; // private
} t_character_cell;

#define SCREEN_CELLS_SIZE (L_WINDOW * H_WINDOW)

typedef struct s_desktop
{
	t_position		 cursor;					// private
	t_character_cell cells[L_WINDOW][H_WINDOW]; // private
} t_desktop;

#define DESKTOP_COUNT 4

typedef struct s_screen_context
{
	t_desktop	   desktops[DESKTOP_COUNT]; // private
	unsigned int   desktop_index;			// private
	unsigned char *vga_buffer;				// private
} t_screen_context;

extern t_screen_context screen_context;

// Init functions
void init_screen_context(t_screen_context *ctx);

// Main functions
void kfs_write_char(t_screen_context *ctx, unsigned char c, unsigned char color);
void clear_screen(t_screen_context *ctx);

#endif
