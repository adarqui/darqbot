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
#ifndef MOD_RANT_H
#define MOD_RANT_H

#include "bot.h"

dlist_t *dl_mod_rant_unique;

module_t mod_rant_info;

bot_t *rant_init(dlist_t *, bot_t *);
bot_t *rant_fini(dlist_t *, bot_t *);
bot_t *rant_help(dlist_t *, bot_t *);
bot_t *rant_run(dlist_t *, bot_t *);
bot_t *rant_input(dlist_t *, bot_t *);

char *rant_change_string(bot_t *, char *, int);
void __rant_init__(void) __attribute__ ((constructor));

enum mod_rant_stuff {
	MOD_RANT_PUSH = 1,
	MOD_RANT_POP,
	MOD_RANT_CLEAR,
	MOD_RANT_SIZE,
	MOD_RANT_LIST,
};

char *rant_op_clear(bot_t *, dlist_t **);
char *rant_op_size(bot_t *, dlist_t **);
char *rant_op_push(bot_t *, dlist_t **, char *);
char *rant_op_pop(bot_t *, dlist_t **);
char *rant_op_list(bot_t *, dlist_t **);

#endif
