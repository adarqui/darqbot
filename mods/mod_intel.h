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
#ifndef MOD_INTEL_H
#define MOD_INTEL_H

#include "bot.h"

module_t mod_intel_info;

bot_t *intel_init(dlist_t *, bot_t *);
bot_t *intel_fini(dlist_t *, bot_t *);
bot_t *intel_help(dlist_t *, bot_t *);
bot_t *intel_run(dlist_t *, bot_t *);

char *intel_change_string(bot_t *, char *, int, char *);

void __intel_init__(void) __attribute__ ((constructor));

enum mod_intel_ops {
	MOD_INTEL_TRUE,
	MOD_INTEL_FALSE,
	MOD_INTEL_SELF,
	MOD_INTEL_STRLEN,
	MOD_INTEL_TOUPPER,
	MOD_INTEL_TOLOWER,
	MOD_INTEL_BZERO,
	MOD_INTEL_MEMSET,
	MOD_INTEL_STRCMP,
	MOD_INTEL_STRNCMP,
	MOD_INTEL_MUL,
	MOD_INTEL_IMUL,
	MOD_INTEL_SUB,
	MOD_INTEL_ADD,
};

int intel_true(void);
int intel_false(void);
void *intel_self(void *);

int intel_strlen(char *);
int intel_strcmp(const char *, const char *);

int intel_toupper(char *);
int intel_tolower(char *);

int intel_bzero(char *, int);
void *intel_memset(void *, int, size_t);

unsigned int intel_mul(int, int);
int intel_imul(int, int);

int intel_sub(int, int);
int intel_add(int, int);

#endif
