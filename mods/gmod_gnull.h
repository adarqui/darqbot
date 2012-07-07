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
#ifndef GMOD_GNULL_H
#define GMOD_GNULL_H

#include "bot.h"

typedef struct gmod_gnull {
	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;

	int null;

} gnull_t;

module_t gmod_gnull_info;

void __gnull_init__(void) __attribute__ ((constructor));

bot_t *gnull_init(dlist_t *, bot_t *);
bot_t *gnull_fini(dlist_t *, bot_t *);
bot_t *gnull_help(dlist_t *, bot_t *);
bot_t *gnull_run(dlist_t *, bot_t *);
bot_t *gnull_control(dlist_t *, bot_t *, int, va_list);

bot_t *gnull_input(dlist_t *, bot_t *);
bot_t *gnull_output(dlist_t *, bot_t *);

bot_t *gnull_destroy_up(dlist_t *, bot_t *);
bot_t *gnull_destroy_down(dlist_t *, bot_t *);

bot_t *gnull_control_up(dlist_t *, bot_t *);
bot_t *gnull_control_down(dlist_t *, bot_t *);

bot_t *gnull_off(dlist_t *, bot_t *);

int gnull_input_client(gnull_t *);
char *gnull_change_string(bot_t *, char *, int);
char *gnull_process_options(gnull_t *, char *);
void gnull_process_options_parse(gnull_t *, char *);
void gnull_process_options_parse_null(gnull_t *, char *);

bot_t *gnull_destroy_up_gnull(gnull_t *);
bot_t *gnull_destroy_down_gnull(gnull_t *);

void gnull_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void gnull_free(void *);

#endif
