#ifndef MOD_FUNINTEL_INTEL2_H
#define MOD_FUNINTEL_INTEL2_H

int intel2_true(void);
int intel2_false(void);
void *intel2_self(void *);
size_t intel2_strlen(const char *);
size_t intel2_strlen2(const char *);
int intel2_strcmp(const char *, const char *);
int intel2_strncmp(const char *, const char *, size_t);
char *intel2_strchr(const char *, int);
char *intel2_strrchr(const char *, int);
char *intel2_strcpy(char *, const char *);
char *intel2_strncpy(char *, const char *, size_t);
void intel2_bzero(void *, size_t);
void *intel2_memset(void *, int, size_t);
unsigned int intel2_mul(unsigned int, unsigned int);
int intel2_imul(int, int);
int intel2_sub(int, int);
int intel2_add(int, int);
unsigned int intel2_div(unsigned int, unsigned int);
int intel2_idiv(int, int);
unsigned int intel2_mod(unsigned int, unsigned int);
int intel2_imod(int, int);
int intel2_neg(int);
unsigned int intel2_eflags(void);
int intel2_and(int, int);
int intel2_or(int, int);
int intel2_xor(int, int);
int intel2_not(int);
int intel2_sar(int, int);
int intel2_sal(int, int);
unsigned int intel2_shr(int, int);
unsigned int intel2_shl(int, int);
int intel2_ror(int, int);
int intel2_rol(int, int);
int intel2_bswap(int);
int intel2_isupper(int);
int intel2_islower(int);
int intel2_toupper(int);
int intel2_tolower(int);
int intel2_isdigit(int);
int intel2_isbinary(int);
int intel2_isalpha(int);
int intel2_isalnum(int);
int intel2_isascii(int);
int intel2_ispunct(int);
int intel2_isblank(int);
int intel2_isspace(int);
int intel2_isxdigit(int);
int intel2_isgraph(int);
int intel2_isprint(int);

/* syscalls */
void intel2_linux_exit(int);

#endif
