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
#ifndef MOD_CMDS_H
#define MOD_CMDS_H

#include "bot.h"

enum mod_cmds_stuff
{
  MOD_CMDS_SIZE,
  MOD_CMDS_GET,
  MOD_CMDS_LIST,
};

module_t mod_cmds_info;

bot_t *cmds_init (dlist_t *, bot_t *);
bot_t *cmds_fini (dlist_t *, bot_t *);
bot_t *cmds_help (dlist_t *, bot_t *);
bot_t *cmds_run (dlist_t *, bot_t *);


char *cmds_change_string (bot_t *, char *, int);

void __cmds_init__ (void) __attribute__ ((constructor));

void cmds_clean_str (char *);

#endif
