#include "bot.h"

size_t c2_strlen(const char *s)
{

	if (*s == '\0')
		return 0;

	return (c2_strlen(s + 1) + 1);
}

char *c2_strchr(const char *s, int c)
{

	if (*s == c)
		return (char *)s;
	else if (*s == '\0')
		return NULL;

	return c2_strchr(s, c);
}

int c2_add(int x, int y)
{

	if (x == 0)
		return y;

	return (c2_add(x < 0 ? x + 1 : x - 1, y + x));
}

char *c2_strcpy(char *s1, const char *s2)
{

	*s1 = *s2;

	if (*s2 == '\0')
		return s1;

	c2_strcpy(s1 + 1, s2 + 1);
	return s1;
}

char *c2_strncpy(char *s1, const char *s2, size_t n)
{

	if (n <= 0 || *s2 == '\0')
		return s1;

	*s1 = *s2;

	c2_strncpy(s1 + 1, s2 + 1, n - 1);
	return s1;
}

void *c2_memset(void *dst, int c, size_t n)
{
	--n;
	*(unsigned char *)(dst + n) = c;
	if ((int)n <= 0)
		return dst;

	return c2_memset(dst, c, n);
}
