#include "print_manager.h"


void print_carriage_return(void)
{
    vga_index += L_WINDOW - (vga_index % L_WINDOW);
}

void print_string(char *str, unsigned char color)
{
    int index = 0;
    while (str[index]) 
    {
        print_char(str[index], color);
        index ++;
    }
}

void print_char(char c, unsigned char color)
{
    terminal_buffer[vga_index] = (unsigned short)c | (unsigned short)color << 8;
    vga_index++;
}

void print_number(int nb, int *count, unsigned char color)
{
	unsigned int	b;

	if (nb < 0)
	{
		b = nb * -1;
        count++;
		print_char('-', color);
	}
	else
	{
		b = nb;
	}
	if (b >= 10)
	{
		print_number(b / 10, &count, color);
		print_number(b % 10, &count, color);
	}
	else
	{
        count++;
		print_char((b + 48), color);
	}
}

void print_hex(int hex, int *count, unsigned char color)
{
    char base[] = "0123456789abcdef";
    if (hex > HEX_BASE_SIZE - 1)
	{
		print_hex(hex / HEX_BASE_SIZE, &count, color);
		hex %= HEX_BASE_SIZE;
	}
    count++;
	print_char(&base[hex], color);
	return (count + 1);
}

int print_k(const char *str, ...)
{
    struct arg_list args;

    char hex_tmp[9];
    init_args(&args, str);
    int total_read = 0;

    while (*str)
    {
        if (*str == '%')
        {
            str++;
            switch (*str)
            {
                case  'd':
                    int count_dec = 0;
                    print_number(get_string_arg(&args), &count_dec, WHITE);
                    total_read += count_dec;
                    break;
                case 's':
                    print_string(get_string_arg(&args), WHITE); // ici on doit compter les charac
                case 'c':
                    print_char(get_char_arg(&args), WHITE);
                    total_read++;
                case 'x':
                    int count_hex = 0;
                    print_hex(get_int_arg(&args), &count_hex, WHITE);
                    total_read += count_hex;

                default:
                    break;
            }
        }
        else
            print_char(*str, WHITE);
            total_read++;
        str++;    
    }
    return total_read;
}