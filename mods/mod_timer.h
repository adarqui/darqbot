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
#ifndef MOD_TIMER_H
#define MOD_TIMER_H

#include "bot.h"

module_t mod_timer_info;

bot_t *timer_init(dlist_t *, bot_t *);
bot_t *timer_fini(dlist_t *, bot_t *);
bot_t *timer_help(dlist_t *, bot_t *);
bot_t *timer_run(dlist_t *, bot_t *);
bot_t *timer_timer(dlist_t *, bot_t *);

void __timer_init__(void) __attribute__ ((constructor));

enum timer_opts {
	TIMER_OPT_ADD = 1,
	TIMER_OPT_GET,
	TIMER_OPT_DEL,
	TIMER_OPT_LIST,
	TIMER_OPT_CLEAR,
};

typedef struct mod_timer_node {
	int id;
	int interval;
	int count;
	time_t last_run;
	char *cmd;
	char *chan;
	bot_t *(*parser) (dlist_t *, bot_t *);
	dlist_t *dlist_node;
	bot_t *bot;
} modtimer_t;

void timer_free(void *);
modtimer_t *timer_alloc(bot_t *, int, int, char *);
char *timer_change_string(dlist_t *, bot_t *, char *, int);

char *timer_op_add(bot_t *, char *);
char *timer_op_del(char *);
char *timer_op_get(char *);
char *timer_op_list(char *);
char *timer_op_clear(char *);

dlist_t *dl_mod_timer;
bit_vector_t *timer_bv;

#endif
