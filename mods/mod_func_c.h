#ifndef MOD_FUNC_C_H
#define MOD_FUNC_C_H

int c_true(void);
int c_false(void);
void *c_self(void *);
size_t c_strlen(const char *);
int c_strcmp(const char *, const char *);
int c_strncmp(const char *, const char *, size_t);
char *c_strchr(const char *, int);
char *c_strrchr(const char *, int);
char *c_strcpy(char *, const char *);
char *c_strncpy(char *, const char *, size_t);
void c_bzero(void *, size_t);
void *c_memset(void *, int, size_t);
unsigned int c_mul(unsigned int, unsigned int);
int c_imul(int, int);
int c_sub(int, int);
int c_add(int, int);
unsigned int c_div(unsigned int, unsigned int);
int c_idiv(int, int);
unsigned int c_mod(unsigned int, unsigned int);
int c_imod(int, int);
int c_neg(int);
unsigned int c_eflags(void);
int c_and(int, int);
int c_or(int, int);
int c_xor(int, int);
int c_not(int);
int c_sar(int, int);
int c_sal(int, int);
unsigned int c_shr(int, int);
unsigned int c_shl(int, int);
int c_ror(int, int);
int c_rol(int, int);
int c_bswap(int);
int c_isupper(int);
int c_islower(int);
int c_toupper(int);
int c_tolower(int);
int c_isdigit(int);
int c_isbinary(int);
int c_isalpha(int);
int c_isalnum(int);
int c_isascii(int);
int c_ispunct(int);
int c_isblank(int);
int c_isspace(int);
int c_isxdigit(int);
int c_isgraph(int);
int c_isprint(int);

#endif
