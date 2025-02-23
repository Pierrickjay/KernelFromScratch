#include "print_manager.h"

int print_string(char *str)
{
	int index = 0;
	while (str[index])
	{
		print_char(str[index]);
		index++;
	}
	return index;
}

void print_char(char c)
{
	kfs_write_char(&screen_context, c);
}

int print_number(int nb)
{
	int	 len_nb = intlen(nb) + 1;
	char str[len_nb];

	itoa(nb, str);
	print_string(str);
	return len_nb;
}

void print_hex(int hex)
{
	char base[] = "0123456789abcdef";
	if (hex > HEX_BASE_SIZE - 1)
	{
		print_hex(hex / HEX_BASE_SIZE);
		hex %= HEX_BASE_SIZE;
	}
	print_char(base[hex]);
}

int print_f(char *str, ...)
{
	int	 *args;
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
				print_char('%');
				total_print++;
				break;
			}
			i++;
			switch (format[i])
			{
				case '%': // Gestion de %%
					print_char('%');
					total_print++;
					break;
				case 'd':
					total_print += print_number(*args++);
					break;
				case 's':
					total_print += print_string(*((char **)args++));
					break;
				case 'c':
					print_char(*args++);
					total_print++;
					break;
				case 'x':
					int count_hex = 0;
					print_hex(*args++);
					total_print += count_hex;
					break;
				default:
					print_char('%');
					print_char(format[i]);
					total_print += 2;
					break;
			}
		}
		else
		{
			print_char(format[i]);
			total_print++;
		}
		i++;
	}
	return total_print;
}
