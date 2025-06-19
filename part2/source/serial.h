#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

// Serial port I/O ports
#define SERIAL_COM1_BASE 0x3F8
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

// Serial port commands
#define SERIAL_LINE_ENABLE_DLAB 0x80

// Initialize the serial port
void serial_init(u16 port);

// Write a character to the serial port
void serial_write_char(u16 port, char c);

// Write a string to the serial port
void serial_write_string(u16 port, const char* str);

// Print an integer to the serial port
int serial_print_number(u16 port, int nb);

#endif 