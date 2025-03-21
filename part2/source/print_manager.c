#include "print_manager.h"

void print_carriage_return(void)
{
	vga_index += L_WINDOW - (vga_index % L_WINDOW);
}

int print_string(char *str, unsigned char color)
{
	int index = 0;
	while (str[index])
	{
		print_char(str[index], color);
		index++;
	}
	return index;
}

void print_char(char c, unsigned char color)
{
	if (c == '\n')
	{
		print_carriage_return();
		return;
	}
	terminal_buffer[vga_index] = (unsigned short)c | (unsigned short)color << 8;
	vga_index++;
}

int print_number(int nb, unsigned char color)
{
	int	 len_nb = intlen(nb) + 1;
	char str[len_nb];

	itoa(nb, str);
	print_string(str, color);
	return len_nb;
}

void print_hex(int hex, unsigned char color)
{
	char base[] = "0123456789abcdef";
	if (hex > HEX_BASE_SIZE - 1)
	{
		print_hex(hex / HEX_BASE_SIZE, color);
		hex %= HEX_BASE_SIZE;
	}
	print_char(base[hex], color);
}

int print_f(char *str, ...)
{
	int	*args;
	char *format;
	int	  i = 0;
	char  tmp_addr[9];
	int	  total_print = 0;

	args   = (int *)(&str);		// pointer of args
	format = (char *)(*args++); // Pointer to char in first string
	i	   = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == '\0') // Si % est le dernier caractère
			{
				print_char('%', WHITE);
				total_print++;
				break;
			}
			i++;
			switch (format[i])
			{
				case '%': // Gestion de %%
					print_char('%', WHITE);
					total_print++;
					break;
				case 'd':
					total_print += print_number(*args++, WHITE);
					break;
				case 's':
					total_print += print_string(*((char **)args++), WHITE);
					break;
				case 'c':
					print_char(*args++, WHITE);
					total_print++;
					break;
				case 'x':
					int count_hex = 0;
					print_hex(*args++, WHITE);
					total_print += count_hex;
					break;
				default:
					print_char('%', WHITE);
					print_char(format[i], WHITE);
					total_print += 2;
					break;
			}
		}
		else
		{
			print_char(format[i], WHITE);
			total_print++;
		}
		i++;
	}
	return total_print;
}
