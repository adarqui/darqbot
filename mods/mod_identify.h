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
#ifndef MOD_IDENTIFY_H
#define MOD_IDENTIFY_H

/*
 *
 *
 */
#include "bot.h"
#include "mongo.h"

void __identify_init__(void) __attribute__ ((constructor));

enum identify_opts {
	IDENTIFY_OPT_RUN = 0,
	IDENTIFY_OPT_GET = 1,
	IDENTIFY_OPT_DEL = 2,
	IDENTIFY_OPT_ADD = 3,
};

module_t mod_identify_info;

bot_t *identify_init(dlist_t *, bot_t *);
bot_t *identify_fini(dlist_t *, bot_t *);
bot_t *identify_help(dlist_t *, bot_t *);
bot_t *identify_run(dlist_t *, bot_t *);

char *identify_change_string(bot_t *, char *, int);

char *identify_op_get(bot_t *, char *);
char *identify_op_del(bot_t *, char *);
char *identify_op_add(bot_t *, char *, char *);
char *identify_op_run(bot_t *);

char *identify_change_string_fn_null(bot_t *, char *, int);
extern char *pcre_change_string(bot_t *, char *, int);

#endif
