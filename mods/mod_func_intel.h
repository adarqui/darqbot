#ifndef MOD_FUNINTEL_INTEL_H
#define MOD_FUNINTEL_INTEL_H

int intel_true(void);
int intel_false(void);
void *intel_self(void *);
size_t intel_strlen(const char *);
int intel_strcmp(const char *, const char *);
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

#endif
