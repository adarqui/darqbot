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
#ifndef MOD_GUILE_H
#define MOD_GUILE_H

#include "bot.h"
#include <libguile.h>

#include <signal.h>

/* libguile/posix.c execl */

enum data_opts {
	DATA_OPT_OUT = 0,
	DATA_OPT_ARG = 1,
};

typedef struct guile_info {

	int pipefds[2];
	dlist_t *text_list;
	bot_t *bot;
	int data_opt;
	int ison;
	int alarm;
} guile_info_t;

guile_info_t guile_info;

void guile_alarm_handler(int);

module_t mod_guile_info;

bot_t *guile_init(dlist_t *, bot_t *);
bot_t *guile_fini(dlist_t *, bot_t *);
bot_t *guile_help(dlist_t *, bot_t *);
bot_t *guile_run(dlist_t *, bot_t *);

char *guile_change_string(bot_t *, char *, int);
static void guile_inner_main(void *, int, char **);

void __guile_init__(void) __attribute__ ((constructor));

#endif
