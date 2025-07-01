#include "string.h"

int strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, int n)
{
	while (n && *s1 && (*s1 == *s2)) {
		s1++;
		s2++;
		n--;
	}
	if (n == 0) {
		return 0; // Strings are equal up to n characters
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}