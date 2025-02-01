#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#define VGA_ADDRESS 0xB8000
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE 15

#define H_WINDOW 25
#define L_WINDOW 80

extern unsigned short* terminal_buffer;
extern unsigned int vga_index;

void clear_screen(void);

void print_string(char *str, unsigned char color);

#endif