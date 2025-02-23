
#include "utils.h"
#include "memory_manager.h"

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