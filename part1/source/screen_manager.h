#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

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

// Window size
#define H_WINDOW 25
#define L_WINDOW 80

extern unsigned short *terminal_buffer;
extern unsigned int	   vga_index;

void clear_screen(void);

#endif