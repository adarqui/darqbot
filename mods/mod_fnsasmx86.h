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
#ifndef MOD_FNSASMX86_H
#define MOD_FNSASMX86_H

#include "bot.h"


enum
{
  MOD_FNSASMX86_TEST = 1,
  MOD_FNSASMX86_ON,
  MOD_FNSASMX86_OFF,
}
 ;

module_t mod_fnsasmx86_info;

bot_t *fnsasmx86_init (dlist_t *, bot_t *);
bot_t *fnsasmx86_fini (dlist_t *, bot_t *);
bot_t *fnsasmx86_help (dlist_t *, bot_t *);
bot_t *fnsasmx86_run (dlist_t *, bot_t *);


char *fnsasmx86_change_string (bot_t * bot, char *, int);
char *fnsasmx86_test (bot_t *);
void fnsasmx86_on (bot_t *);
void fnsasmx86_off (bot_t *);


int asmx86_true (void);
int asmx86_false (void);
int asmx86_val_int (int val);
char *asmx86_val_string (char *val);
size_t asmx86_strlen (const char *);
void *asmx86_memset (void *, int, size_t);
void asmx86_bzero (void *, size_t);
int asmx86_strcmp (const char *, const char *);
int asmx86_strncmp (const char *, const char *, size_t);


void __fnsasmx86_init__ (void) __attribute__ ((constructor));

#endif
