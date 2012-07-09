/*
     Copyright (C) 2012 Andrew Darqui

     This file is part of darqbot.

     darqbot is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     darqbot is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with darqbot.  If not, see <http://www.gnu.org/licenses/>.

     Contact: [website: http://www.adarq.org]
      email: andrew.darqui@g m a i l . c o m
*/
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#ifndef MOD_FUNC_H
#define MOD_FUNC_H

#include "bot.h"
#include "mod_func_c.h"
#include "mod_func_intel.h"

module_t mod_func_info;

bot_t *func_init(dlist_t *, bot_t *);
bot_t *func_fini(dlist_t *, bot_t *);
bot_t *func_help(dlist_t *, bot_t *);
bot_t *func_run(dlist_t *, bot_t *);

char *func_change_string(bot_t *, char *, int, char *);

void __func_init__(void) __attribute__ ((constructor));

enum mod_func_ops {
	MOD_FUNC_SWITCH = 1,
	MOD_FUNC_TRUE,
	MOD_FUNC_FALSE,
	MOD_FUNC_SELF,
	MOD_FUNC_STRLEN,
	MOD_FUNC_BZERO,
	MOD_FUNC_MEMSET,
	MOD_FUNC_EFLAGS,
	MOD_FUNC_STRCMP,
	MOD_FUNC_STRNCMP,
	MOD_FUNC_MUL,
	MOD_FUNC_IMUL,
	MOD_FUNC_SUB,
	MOD_FUNC_ADD,
	MOD_FUNC_DIV,
	MOD_FUNC_IDIV,
	MOD_FUNC_MOD,
	MOD_FUNC_IMOD,
	MOD_FUNC_NEG,
	MOD_FUNC_AND,
	MOD_FUNC_OR,
	MOD_FUNC_XOR,
	MOD_FUNC_NOT,
	MOD_FUNC_JMP,
	MOD_FUNC_SAR,
	MOD_FUNC_SHR,
	MOD_FUNC_SAL,
	MOD_FUNC_SHL,
	MOD_FUNC_ROR,
	MOD_FUNC_ROL,
	MOD_FUNC_BSWAP,
	MOD_FUNC_ISUPPER,
	MOD_FUNC_ISLOWER,
	MOD_FUNC_TOUPPER,
	MOD_FUNC_TOLOWER,
};

void func_switch(char *);

int (*func_true) (void);
int (*func_false) (void);
void *(*func_self) (void *);

size_t(*func_strlen) (const char *);
int (*func_strcmp) (const char *, const char *);

void (*func_bzero) (void *, size_t);
void *(*func_memset) (void *, int, size_t);

unsigned int (*func_mul) (unsigned int, unsigned int);
int (*func_imul) (int, int);

int (*func_sub) (int, int);
int (*func_add) (int, int);

unsigned int (*func_div) (unsigned int, unsigned int);
int (*func_idiv) (int, int);
unsigned int (*func_mod) (unsigned int, unsigned int);
int (*func_imod) (int, int);

int (*func_neg) (int);
unsigned int (*func_eflags) (void);

int (*func_and) (int, int);
int (*func_or) (int, int);
int (*func_xor) (int, int);
int (*func_not) (int);

int (*func_sar) (int, int);
unsigned int (*func_shr) (int, int);
int (*func_sal) (int, int);
unsigned int (*func_shl) (int, int);
int (*func_ror) (int, int);
int (*func_rol) (int, int);

int (*func_bswap) (int);

int (*func_isupper) (int);
int (*func_islower) (int);
int (*func_toupper) (int);
int (*func_tolower) (int);

#endif
