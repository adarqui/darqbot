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
#ifndef MOD_QUOTE_H
#define MOD_QUOTE_H

#include "bot.h"


module_t mod_quote_info;

bot_t *quote_init (dlist_t *, bot_t *);
bot_t *quote_fini (dlist_t *, bot_t *);
bot_t *quote_help (dlist_t *, bot_t *);
bot_t *quote_run (dlist_t *, bot_t *);

char *quote_change_string (char *, char *);

void __quote_init__ (void) __attribute__ ((constructor));


dlist_t *dl_quote_pairs;

typedef struct quote
{
  char title[32];
  char filename[132];
  int count;
} quote_t;

void quote_add_files_to_db_file (char *);
void quote_add_files_to_db (void);
char *quote_get (xdb_pair_t *, char *);

#endif
