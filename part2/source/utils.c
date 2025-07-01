
#include "utils.h"
#include "memory_manager.h"

void init_position(t_position *pos)
{
	pos->x = 0;
	pos->y = 0;
}

int atoi(const char *str)
{
	int sign = 1;
	int result = 0;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r')) {
		str++;
	}
	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}
	while (*str >= '0' && *str <= '9') {
		result = result * 10 + (*str - '0');
		str++;
	}
	return result * sign;
}

void itoa(int n, char *str)
{
	int nb;
	int i;
	int len;

	nb	= n;
	len = intlen(n);
	ft_memset(str, 0, len + 1);
	if (nb < 0) {
		nb = -nb;
	}
	i = len - 1;
	if (nb == 0) {
		str[i] = '0';
		return;
	}
	while (nb != 0) {
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
	if (n < 0) {
		n = -n;
		i++;
	}
	while (n >= 10) {
		n = n / 10;
		i++;
	}
	return (i);
}
