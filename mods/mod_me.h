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
#ifndef MOD_ME_H
#define MOD_ME_H

#include "bot.h"
#include <matheval.h>

module_t mod_me_info;

bot_t *me_init (dlist_t *, bot_t *);
bot_t *me_fini (dlist_t *, bot_t *);
bot_t *me_help (dlist_t *, bot_t *);
bot_t *me_run (dlist_t *, bot_t *);

void __me_init__ (void) __attribute__ ((constructor));

typedef struct me
{
  double x;
  char x_set;
  double y;
  char y_set;
  double z;
  char z_set;
} me_t;

char *me_change_string (char *, int, me_t *);
void me_parse_options (char *, me_t *);

#endif
