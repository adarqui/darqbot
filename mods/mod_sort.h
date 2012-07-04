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
#ifndef MOD_SORT_H
#define MOD_SORT_H

#include "bot.h"


enum
{
  SORT_FORW = 1,
  SORT_BACKW = 2,
  SORT_RANDW = 3,
  SORT_FORC = 4,
  SORT_BACKC = 5,
  SORT_RANDC = 6
};

module_t mod_sort_info;

bot_t *sort_init (dlist_t *, bot_t *);
bot_t *sort_fini (dlist_t *, bot_t *);
bot_t *sort_help (dlist_t *, bot_t *);
bot_t *sort_run (dlist_t *, bot_t *);

void __sort_init__ (void) __attribute__ ((constructor));

char *sort_change_string (char *, int, int, char *);

#endif
