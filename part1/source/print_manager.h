#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#define HEX_BASE_SIZE 16

#include "screen_manager.h"
#include "utils.h"

void print_number(int nb,int *count, unsigned char color);
void print_char(char c, unsigned char color);
int print_string(char *str, unsigned char color);
void print_carriage_return(void);
int printf(char *str, ...);


#endif

