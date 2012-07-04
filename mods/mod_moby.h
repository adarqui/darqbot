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
#ifndef MOD_MOBY_H
#define MOD_MOBY_H

#include "bot.h"



enum
{
  MOBY_SINGLE = 1,
  MOBY_LIST
};

module_t mod_moby_info;

bot_t *moby_init (dlist_t *, bot_t *);
bot_t *moby_fini (dlist_t *, bot_t *);
bot_t *moby_help (dlist_t *, bot_t *);
bot_t *moby_run (dlist_t *, bot_t *);

typedef struct moby_info
{

  int initialized;
  char db_name[132];
  int count;

} moby_info_t;

moby_info_t moby_info;


char *moby_change_string (char *, int);
void moby_create_db (bot_t *);
char *moby_get_syn (DB *, char *, int);
int moby_transform_word (char *, char *);


void __moby_init__ (void) __attribute__ ((constructor));

#endif
