
#include "utils.h"
#include "memory_manager.h"
#include "screen_manager.h"

void init_position(t_position *pos)
{
	pos->x = 0;
	pos->y = 0;
}

void itoa(int n, char *str)
{
	int nb;
	int i;
	int len;

	nb	= n;
	len = intlen(n);
	ft_memset(str, 0, len + 1);
	if (nb < 0)
	{
		nb = -nb;
	}
	i = len - 1;
	if (nb == 0)
	{
		str[i] = '0';
		return;
	}
	while (nb != 0)
	{
		str[i--] = (nb % 10) + '0';
		nb		 = nb / 10;
	}
	if (n < 0)
		str[i] = '-';
}

int intlen(int n)
{
	int i;

	i = 1;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n >= 10)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

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
