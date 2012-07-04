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
#ifndef MOD_ALIAS_H
#define MOD_ALIAS_H

#include "bot.h"
#include "mongo.h"


enum alias_opts
{
  ALIAS_OPT_RUN = 0,
  ALIAS_OPT_GET = 1,
  ALIAS_OPT_DEL = 2,
  ALIAS_OPT_ADD = 3,
  ALIAS_OPT_GETMORE = 4,
};

module_t mod_alias_info;

bot_t *alias_init (dlist_t *, bot_t *);
bot_t *alias_fini (dlist_t *, bot_t *);
bot_t *alias_help (dlist_t *, bot_t *);
bot_t *alias_run (dlist_t *, bot_t *);


void __alias_init__ (void) __attribute__ ((constructor));


char *alias_change_string (dlist_t *, bot_t *, int, char *);


char *alias_op_run (dlist_t *, bot_t *, char *);
char *alias_op_get (bot_t *, char *);
char *alias_op_del (bot_t *, char *);
char *alias_op_add (bot_t *, char *, char *);
char *alias_op_getmore (bot_t *, char *);

#endif
