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
#ifndef MOD_MAP_H
#define MOD_MAP_H

#include "bot.h"

module_t mod_map_info;

bot_t *map_init(dlist_t *, bot_t *);
bot_t *map_fini(dlist_t *, bot_t *);
bot_t *map_help(dlist_t *, bot_t *);
bot_t *map_run(dlist_t *, bot_t *);

char *map_change_string(dlist_t *, bot_t *, char *, int, char *, char *,
			char *);

void __map_init__(void) __attribute__ ((constructor));

enum mod_map_opts {
	MAP_OPT_NORMAL = 1,
};

#endif
