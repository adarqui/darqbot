#ifndef MOD_FUNINTEL_INTEL_H
#define MOD_FUNINTEL_INTEL_H

int intel_true(void);
int intel_false(void);
void *intel_self(void *);
size_t intel_strlen(const char *);
size_t intel_strlen2(const char *);
int intel_strcmp(const char *, const char *);
int intel_strncmp(const char *, const char *, size_t);
char *intel_strchr(const char *, int);
char *intel_strrchr(const char *, int);
char *intel_strcpy(char *, const char *);
char *intel_strncpy(char *, const char *, size_t);
void intel_bzero(void *, size_t);
void *intel_memset(void *, int, size_t);
unsigned int intel_mul(unsigned int, unsigned int);
int intel_imul(int, int);
int intel_sub(int, int);
int intel_add(int, int);
unsigned int intel_div(unsigned int, unsigned int);
int intel_idiv(int, int);
unsigned int intel_mod(unsigned int, unsigned int);
int intel_imod(int, int);
int intel_neg(int);
unsigned int intel_eflags(void);
int intel_and(int, int);
int intel_or(int, int);
int intel_xor(int, int);
int intel_not(int);
int intel_sar(int, int);
int intel_sal(int, int);
unsigned int intel_shr(int, int);
unsigned int intel_shl(int, int);
int intel_ror(int, int);
int intel_rol(int, int);
int intel_bswap(int);
int intel_isupper(int);
int intel_islower(int);
int intel_toupper(int);
int intel_tolower(int);
int intel_isdigit(int);
int intel_isbinary(int);
int intel_isalpha(int);
int intel_isalnum(int);
int intel_isascii(int);
int intel_ispunct(int);
int intel_isblank(int);
int intel_isspace(int);
int intel_isxdigit(int);
int intel_isgraph(int);
int intel_isprint(int);

/* syscalls */
void intel_linux_exit(int);

#endif
