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
#ifndef MOD_UU_H
#define MOD_UU_H

#include "bot.h"


enum
{
  MOD_UU_ENC = 1,
  MOD_UU_DEC = 2,
};

module_t mod_uu_info;

bot_t *uu_init (dlist_t *, bot_t *);
bot_t *uu_fini (dlist_t *, bot_t *);
bot_t *uu_help (dlist_t *, bot_t *);
bot_t *uu_run (dlist_t *, bot_t *);


char *uu_change_string (char *, int, int *);

char *xxencode (char *, int);
int xxdecode (char *, int *, int *);

void __uu_init__ (void) __attribute__ ((constructor));

#endif
