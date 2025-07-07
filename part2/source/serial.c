#include "serial.h"
#include "io.h"
#include "utils.h"
#include "print_manager.h" 

static const char HEX_DIGITS[] = "0123456789abcdef";

void serial_init(u16 port) {
    // Disable interrupts
    outb(SERIAL_FIFO_COMMAND_PORT(port), 0x00);
    
    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_LINE_COMMAND_PORT(port), SERIAL_LINE_ENABLE_DLAB);
    
    // Set divisor to 3 (38400 baud)
    outb(SERIAL_DATA_PORT(port), 0x03);
    outb(SERIAL_DATA_PORT(port) + 1, 0x00);
    
    // 8 bits, no parity, one stop bit
    outb(SERIAL_LINE_COMMAND_PORT(port), 0x03);
    
    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_FIFO_COMMAND_PORT(port), 0xC7);
    
    // IRQs enabled, RTS/DSR set
    outb(SERIAL_MODEM_COMMAND_PORT(port), 0x0B);
}

void serial_write_char(u16 port, char c) {
    // Wait until the transmit buffer is empty
    while ((inb(SERIAL_LINE_STATUS_PORT(port)) & 0x20) == 0);
    
    // Send the character
    outb(SERIAL_DATA_PORT(port), c);
}

void serial_write_string(u16 port, const char* str) {
    while (*str) {
        serial_write_char(port, *str++);
    }
} 

int serial_print_number(u16 port, int nb)
{
	int	 len_nb = intlen(nb) + 1;
	char str[len_nb];

	itoa(nb, str);
	serial_write_string(port, str);
	return len_nb;
}

void print_hex_serial(int hex)
{
	if (hex > HEX_BASE_SIZE - 1) {
		print_hex_serial(hex / HEX_BASE_SIZE);
		hex %= HEX_BASE_SIZE;
	}
	serial_write_char(SERIAL_COM1_BASE, HEX_DIGITS[hex]);
}

void print_serial(char *str, ...)
{
	int	 *args;
	char *format;
	int	  i = 0;
	char  tmp_addr[9];

	args   = (int *)(&str);		// pointer of args
	format = (char *)(*args++); // Pointer to char in first string
	i	   = 0;

	while (*format) {
		if (*format == '%') {
			format++;
			switch (*format) {
				case '%':
					serial_write_char(SERIAL_COM1_BASE, '%');
					break;
				case 'd':
					serial_print_number(SERIAL_COM1_BASE, *args++);
					break;
				case 's':
					serial_write_string(SERIAL_COM1_BASE, *((char **)args++));
					break;
				case 'c':
					serial_write_char(SERIAL_COM1_BASE, *args);
					args++;					
					break;
				case 'x':
					print_hex_serial(*args++);
					break;
				default:
					serial_write_char(SERIAL_COM1_BASE, '%');
					serial_write_char(SERIAL_COM1_BASE, *format);
					break;
			}
		}
		else {
			serial_write_char(SERIAL_COM1_BASE, *format);			
		}
		format++;
	}
}
