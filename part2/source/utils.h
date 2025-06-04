#ifndef UTILS_H
#define UTILS_H


#include <stddef.h>


typedef struct s_position
{
	unsigned int x;
	unsigned int y;
} t_position;

void init_position(t_position *pos);

void itoa(int n, char *str);
int	 intlen(int n);
unsigned char get_log_color(const char *level);

#endif
