#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#define HEX_BASE_SIZE 16

#include "screen_manager.h"
#include "utils.h"

int	 print_number(int nb, unsigned char color);
int	 print_string(char *str, unsigned char color);

int print_f(char *str, ...);

#endif
