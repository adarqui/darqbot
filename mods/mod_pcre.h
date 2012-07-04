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
#ifndef MOD_PCRE_H
#define MOD_PCRE_H

/* man pcrepattern */
#include "bot.h"

#include <pcre.h>


enum pcre_opt_flags
{
  OPT_NOCASE = 0x01,
  OPT_EXTENDED = 0x02,
  OPT_REMOVE = 0x04,
  OPT_EXTRACT = 0x08,
  OPT_UNGREEDY = 0x10,
  OPT_REMOVELINE = 0x20,
};

module_t mod_pcre_info;

bot_t *pcre_init (dlist_t *, bot_t *);
bot_t *pcre_fini (dlist_t *, bot_t *);
bot_t *pcre_help (dlist_t *, bot_t *);
bot_t *pcre_run (dlist_t *, bot_t *);

char *pcre_change_string (bot_t *, char *, int);

void __pcre_init__ (void) __attribute__ ((constructor));

#endif
