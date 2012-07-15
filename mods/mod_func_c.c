#include "bot.h"

#include "mod_func.h"

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

	if (!_sNULL((char *)s))
		return 0;

	while (*s)
		s++;

	return (size_t) (s - base);
}

int c_strcmp(const char *s1, const char *s2)
{
	size_t ret = 0;

	if (!_sNULL((char *)s1) || !_sNULL((char *)s2)) {
		return -1;
	}

	while ((*s1 && *s2) && (*s1 == *s2)) {
		s1++;
		s2++;
	}

	if (*s1 < *s2)
		ret = -1;
	else if (*s1 > *s2)
		ret = 1;

	return ret;
}

int c_strncmp(const char *s1, const char *s2, size_t n)
{

	if (!_sNULL((char *)s1) || !_sNULL((char *)s2) || n <= 0)
		return -1;
	int ret = 0;

	while ((*s1 && *s2) && (*s1 == *s2) && ((int)--n > 0)) {
		s1++;
		s2++;
	}

	if (*s1 < *s2)
		ret = -1;
	else if (*s1 > *s2)
		ret = 1;
	return ret;
}

char *c_strchr(const char *s, int c)
{

	if (!s)
		return NULL;

	while (*s) {
		if (*s == c)
			return (char *)s;
		s++;
	}

	return NULL;
}

char *c_strrchr(const char *s, int c)
{
	int len;

	if (!s)
		return NULL;

	len = strlen(s) - 1;
	for (s = s + strlen(s); len >= 0; len--) {
		if (*(s - len) == c)
			return ((char *)s - len);
	}

	return NULL;
}

char *c_strcpy(char *s1, const char *s2)
{
	int n;

	for (n = 0; s1[n] = s2[n], s2[n] != '\0'; n++) ;

	return s1;
}

char *c_strncpy(char *s1, const char *s2, size_t n)
{

	while ((int)--n >= 0) {
		s1[n] = s2[n];
	}

	return s1;
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

	n = n - 1;
	if (!v || n <= 0)
		return v;

	while ((int)n >= 0) {
		*(char *)(v + n) = (int)c;
		n--;
	}

	return v;
}

unsigned int c_mul(unsigned int x, unsigned int y)
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

unsigned int c_mod(unsigned int x, unsigned int y)
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

char * c_eflags(void)
{
dlist_t * dl_text=NULL;
char * str=NULL;

/*
   FLAGS
   0      CF                                   Carry flag                                               S
   1      1                                    Reserved
   2      PF                                   Parity flag                                              S
   3      0                                    Reserved
   4      AF                                   Adjust flag                                              S
   5      0                                    Reserved
   6      ZF                                   Zero flag                                                S
   7      SF                                   Sign flag                                                S
   8      TF                                   Trap flag (single step)                                  X
   9      IF                                   Interrupt enable flag                                    C
   10     DF                                   Direction flag                                           C
   11     OF                                   Overflow flag                                            S
   12, 13 1,1 / IOPL                           I/O privilege level (286+ only) always 1 on 8086 and 186 X
   14     1 / NT                               Nested task flag (286+ only) always 1 on 8086 and 186    X
   15     1 on 8086 and 186, should be 0 above Reserved
   EFLAGS
   16     RF                                   Resume flag (386+ only)                                  X
   17     VM                                   Virtual 8086 mode flag (386+ only)                       X
   18     AC                                   Alignment check (486SX+ only)                            X
   19     VIF                                  Virtual interrupt flag (Pentium+)                        X
   20     VIP                                  Virtual interrupt pending (Pentium+)                     X
   21     ID                                   Able to use CPUID instruction (Pentium+)                 X
   22     0                                    Reserved
   23     0                                    Reserved
   24     0                                    Reserved
   25     0                                    Reserved
   26     0                                    Reserved
   27     0                                    Reserved
   28     0                                    Reserved
   29     0                                    Reserved
   30     0                                    Reserved
   31     0                                    Reserved
   RFLAGS
   32-63  0                                    Reserved
*/


dl_str_unite(&dl_text, 
"|CF|1|PF|0|AF|0|ZF|SF|TF|IF|DF|OF|1|1|NT|1|RF|VM|AC|VIF|VIP|ID|0... 0x%.4x [%u]\n", i_eflags, (unsigned long)i_eflags);
dl_str_unite(&dl_text, 
"|%.2x|1|%.2x|0|%.2x|0|%.2x|%.2x|%.2x|%.2x|%.2x|%.2x|1|1|%.2x|1|%.2x|%.2x|%.2x|%.3x|%.3x|%.2x|0... 0x%.4x [%u]\n", 
(i_eflags >> 0) & 0x01 ? 1 : 0,
(i_eflags >> 2) & 0x01 ? 1 : 0, 
(i_eflags >> 4) & 0x01 ? 1 : 0,
(i_eflags >> 6) & 0x01 ? 1 : 0,
(i_eflags >> 7) & 0x01 ? 1 : 0,
(i_eflags >> 8) & 0x01 ? 1 : 0,
(i_eflags >> 9) & 0x01 ? 1 : 0,
(i_eflags >> 10) & 0x01 ? 1 : 0,
(i_eflags >> 11) & 0x01 ? 1 : 0,
(i_eflags >> 14) & 0x01 ? 1 : 0,
(i_eflags >> 16) & 0x01 ? 1 : 0,
(i_eflags >> 17) & 0x01 ? 1 : 0,
(i_eflags >> 18) & 0x01 ? 1 : 0,
(i_eflags >> 19) & 0x01 ? 1 : 0,
(i_eflags >> 20) & 0x01 ? 1 : 0,
(i_eflags >> 21) & 0x01 ? 1 : 0,
i_eflags, (unsigned long)i_eflags);

str = dlist_to_str(dl_text);
dl_str_destroy(&dl_text);

	return str ;
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
/* fix */
	return x >> y;
}

int c_rol(int x, int y)
{
/* fix */
	return x << y;
}

int c_bswap(int x)
{
	return htonl(x);
}

int c_bsf(int x) {
int i, pos=0;

for(i=0;i<31;i++) {
if((x >> i) & 0x01) { pos = i; break; }
}

return pos;
}

int c_bsr(int x) {
int i, pos = 0;
for(i=31;i>=0;i--) {
if((x >> i) & 0x01) { pos = i; break; }
}

return pos;
}


int c_bt(int x, int y) {

if((x >> y) & 0x01) return 1;

return 0;
}


int c_btc(int x, int y) {
int z=0;

z = ((x >> y) & 0x01);
if(z) {
x = x & ~(0x01 << y);
}
else
{
x = x | (0x01 << y);
}

return x;
}


int c_btr(int x, int y) {
x = x & ~(0x01 << y);

return x;
}


int c_bts(int x, int y) {
x = x | (0x01 << y);

return x;
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

int c_isdigit(int x)
{
	if (x >= 48 && x <= 57)
		return 1;

	return 0;
}

int c_isbinary(int x)
{
	if (x == 48 || x == 49)
		return 1;

	return 0;
}

int c_isalpha(int x)
{

	if ((x >= 65 && x <= 90) || (x >= 97 && x <= 122))
		return 1;

	return 0;
}

int c_isalnum(int x)
{

	if ((x >= 48 && x <= 57) || (x >= 65 && x <= 90)
	    || (x >= 97 && x <= 122))
		return 1;

	return 0;
}

int c_isascii(int x)
{
	return 0;
}

int c_ispunct(int x)
{

	return 0;
}

int c_isblank(int x)
{

	if (x == 9 || x == 32)
		return 1;

	return 0;
}

int c_isspace(int x)
{

	if (x >= 9 && x <= 13)
		return 1;

	return 0;
}

int c_isprint(int x)
{

	return 0;
}

int c_isxdigit(int x)
{

	if ((x >= 48 && x <= 57) || (x >= 65 && x <= 70)
	    || (x >= 97 && x <= 102))
		return 1;

	return 0;
}

int c_isgraph(int x)
{

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
