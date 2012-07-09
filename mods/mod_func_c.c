#include "bot.h"

int c_true(void)
{
	return 1;
}

int c_false(void)
{
	return 0;
}

void *c_self(void *s)
{
	return s;
}

size_t c_strlen(const char *s)
{
	char *base = (char *)s;

	if (!sNULL((char *)s))
		return 0;

	while (*s)
		s++;

	return (size_t) (s - base);
}

int c_strcmp(const char *s1, const char *s2)
{
	size_t ret;

	if (!sNULL((char *)s1) || !sNULL((char *)s2)) {
		return -1;
	}

	while ((*s1 && *s2) && (*s1 == *s2)) {
		s1++;
		s2++;
	}

	ret = 0;
	if (*s1 < *s2)
		ret = -1;
	else if (*s1 > *s2)
		ret = 1;

	return ret;
}

void c_bzero(void *v, size_t n)
{

	if (!v || n <= 0)
		return;

	while (n >= 0) {
		*(char *)v = 0;
		n--;
	}

	return;
}

void *c_memset(void *v, int c, size_t n)
{

	if (!v || n <= 0)
		return v;

	while ((int)n >= 0) {
		*(char *)(v + n) = (int)c;
		n--;
	}

	return v;
}

unsigned int c_mul(int x, int y)
{

	return x * y;
}

int c_imul(int x, int y)
{
	return x * y;
}

int c_sub(int x, int y)
{
	return x - y;
}

int c_add(int x, int y)
{
	return x + y;
}

unsigned c_div(unsigned int x, unsigned int y)
{
	return x / y;
}

int c_idiv(int x, int y)
{
	return x / y;
}

unsigned int c_mod(int x, int y)
{
	return x % y;
}

int c_imod(int x, int y)
{
	return x % y;
}

int c_neg(int x)
{
	return x * (-1);
}

unsigned int c_eflags(void)
{
	return 1;
}

int c_and(int x, int y)
{
	return x & y;
}

int c_or(int x, int y)
{
	return x | y;
}

int c_xor(int x, int y)
{
	return x ^ y;
}

int c_not(int x)
{
	return ~x;
}

int c_sar(int x, int y)
{
	return x >> y;
}

int c_sal(int x, int y)
{
	return x << y;
}

unsigned c_shr(int x, int y)
{
	return x >> y;
}

unsigned c_shl(int x, int y)
{
	return x << y;
}

int c_ror(int x, int y)
{
	return x >> y;
}

int c_rol(int x, int y)
{
	return x << y;
}

int c_bswap(int x)
{
	return htonl(x);
}

int c_isupper(int x)
{

	if (x >= 65 && x <= 90)
		return 1;

	return 0;
}

int c_islower(int x)
{

	if (x >= 97 && x <= 122)
		return 1;

	return 0;
}

int c_toupper(int x)
{
	if (c_islower(x)) {
		return x - 32;
	}

	return x;
}

int c_tolower(int x)
{
	if (c_isupper(x)) {
		return x + 32;
	}

	return x;
}
