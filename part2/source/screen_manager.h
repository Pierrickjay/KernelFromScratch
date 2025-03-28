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

#define KERN_EMERG "<0>"   // System is unusable
#define KERN_ALERT "<1>"   // Action must be taken immediately
#define KERN_CRIT "<2>"	   // Critical conditions
#define KERN_ERR "<3>"	   // Error conditions
#define KERN_WARNING "<4>" // Warning conditions
#define KERN_NOTICE "<5>"  // Normal but significant condition
#define KERN_INFO "<6>"	   // Informational
#define KERN_DEBUG "<7>"   // Debug-level messages

extern unsigned short *terminal_buffer;
extern unsigned int	   vga_index;

void clear_screen(void);

#endif
