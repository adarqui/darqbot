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
#ifndef MOD_MONGOTESTFMT_H
#define MOD_MONGOTESTFMT_H

#include "bot.h"


enum
{
  MONGOTESTFMT_ADD = 1,
  MONGOTESTFMT_DEL = 2,
  MONGOTESTFMT_GET = 3,
  MONGOTESTFMT_UPDATE = 4,
  MONGOTESTFMT_TEST = 5,
  MONGOTESTFMT_ADDBSON = 6,
  MONGOTESTFMT_GETBSON = 7,
}
 ;

module_t mod_mongotestfmt_info;

bot_t *mongotestfmt_init (dlist_t *, bot_t *);
bot_t *mongotestfmt_fini (dlist_t *, bot_t *);
bot_t *mongotestfmt_help (dlist_t *, bot_t *);
bot_t *mongotestfmt_run (dlist_t *, bot_t *);


char *mongotestfmt_change_string (bot_t *, char *, int);


void __mongotestfmt_init__ (void) __attribute__ ((constructor));

#endif
