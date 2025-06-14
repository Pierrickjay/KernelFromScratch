#include "print_manager.h"

unsigned char get_log_color(const char *level)
{
	switch (level[1])
	{
		case '0':
			return RED; // EMERG  (most critical)
		case '1':
			return RED; // ALERT
		case '2':
			return RED; // CRIT
		case '3':
			return YELLOW; // ERR  Red
		case '4':
			return YELLOW; // WARNING
		case '5':
			return MAGENTA; // NOTICE
		case '6':
			return MAGENTA; // INFO
		case '7':
			return WHITE; // DEBUG
		default:
			return WHITE;
	}
}

int print_string(char *str)
{
	int index = 0;
	while (str[index])
	{
		kfs_write_char(&screen_context, str[index]);
		index++;
	}
	return index;
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
	char *base = "0123456789abcdef";
	if (hex > HEX_BASE_SIZE - 1)
	{
		print_hex(hex / HEX_BASE_SIZE);
		hex %= HEX_BASE_SIZE;
	}
	kfs_write_char(&screen_context, base[hex]);
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
				kfs_write_char(&screen_context, '%');
				total_print++;
				break;
			}
			i++;
			switch (format[i])
			{
				case '%': // Gestion de %%
					kfs_write_char(&screen_context, '%');
					total_print++;
					break;
				case 'd':
					total_print += print_number(*args++);
					break;
				case 's':
					total_print += print_string(*((char **)args++));
					break;
				case 'c':
					kfs_write_char(&screen_context, *args++);
					total_print++;
					break;
				case 'x':
					int count_hex = 0;
					print_hex(*args++);
					total_print += count_hex;
					break;
				default:
					kfs_write_char(&screen_context, '%');
					kfs_write_char(&screen_context, format[i]);
					total_print += 2;
					break;
			}
		}
		else
		{
			kfs_write_char(&screen_context, format[i]);
			total_print++;
		}
		i++;
	}
	return total_print;
}

int print_k(const char *format, ...)
{
	int *args;
	int	 i = 0;
	char tmp_addr[9];

	args   = (int *)(&format);	// pointer of args
	format = (char *)(*args++); // Pointer to char in first string
	i	   = 0;

	int			  total_print = 0;
	unsigned char color		  = WHITE;

	// Check if the format starts with a log level
	if (format[0] == '<' && format[2] == '>')
	{
		color = get_log_color(format);
		format += 3; // Skip the log level prefix
	}

	// Use existing print_f logic with the specified color
	char *fmt_copy = (char *)format;
	int	 *arg_ptr  = (int *)(&args);

	set_color(&screen_context, color);
	while (*fmt_copy)
	{
		if (*fmt_copy == '%')
		{
			fmt_copy++;
			switch (*fmt_copy)
			{
				case '%':
					kfs_write_char(&screen_context, '%');
					total_print++;
					break;
				case 'd':
					total_print += print_number(*arg_ptr++);
					break;
				case 's':
					total_print += print_string(*((char **)arg_ptr++));
					break;
				case 'c':
					kfs_write_char(&screen_context, *arg_ptr++);
					total_print++;
					break;
				case 'x':
					print_hex(*arg_ptr++);
					total_print++;
					break;
				default:
					kfs_write_char(&screen_context, '%');
					kfs_write_char(&screen_context, *fmt_copy);
					total_print += 2;
					break;
			}
		}
		else
		{
			kfs_write_char(&screen_context, *fmt_copy);
			total_print++;
		}
		fmt_copy++;
	}
	return total_print;
}
