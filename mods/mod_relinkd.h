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
#ifndef MOD_RELINKD_H
#define MOD_RELINKD_H

#include "bot.h"

module_t mod_relinkd_info;

bot_t *relinkd_init(dlist_t *, bot_t *);
bot_t *relinkd_fini(dlist_t *, bot_t *);
bot_t *relinkd_help(dlist_t *, bot_t *);
bot_t *relinkd_run(dlist_t *, bot_t *);

char *relinkd_change_string(bot_t *, char *, int, int);
void __relinkd_init__(void) __attribute__ ((constructor));

enum relinkd_opts {
	RELINKD_OPT_QUERY_LINKS = 1,
	RELINKD_OPT_QUERY_SUBS,
	RELINKD_OPT_QUERY_RELOAD,
	RELINKD_OPT_DESTROY_LINK,
	RELINKD_OPT_DESTROY_SUB,
	RELINKD_OPT_DESTROY_ALL,
};

char *relinkd_op_query_links(bot_t *, int);
char *relinkd_op_query_subs(bot_t *, int);
char *relinkd_op_query_reload_modules(bot_t *, int);
char *relinkd_op_destroy_link(bot_t *, int, int);
char *relinkd_op_destroy_sub(bot_t *, int, int);
char *relinkd_op_destroy_all(bot_t *, int);
char *relinkd_op_reload(bot_t *, int);

#endif
