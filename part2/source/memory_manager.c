#include "memory_manager.h"

void *ft_memset(void *s, int c, int n)
{
	char *str;
	int	  i;

	i	= 0;
	str = s;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (s);
}