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
#ifndef GMOD_GFDPASSC_H
#define GMOD_GFDPASSC_H

#include "bot.h"

typedef struct gmod_gfdpassc {
	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;

	int null;

	int fd;

} gfdpassc_t;

module_t gmod_gfdpassc_info;

void __gfdpassc_init__(void) __attribute__ ((constructor));

bot_t *gfdpassc_init(dlist_t *, bot_t *);
bot_t *gfdpassc_fini(dlist_t *, bot_t *);
bot_t *gfdpassc_help(dlist_t *, bot_t *);
bot_t *gfdpassc_run(dlist_t *, bot_t *);
bot_t *gfdpassc_control(dlist_t *, bot_t *, int, va_list);

bot_t *gfdpassc_input(dlist_t *, bot_t *);
bot_t *gfdpassc_output(dlist_t *, bot_t *);

bot_t *gfdpassc_destroy_up(dlist_t *, bot_t *);
bot_t *gfdpassc_destroy_down(dlist_t *, bot_t *);

bot_t *gfdpassc_control_up(dlist_t *, bot_t *);
bot_t *gfdpassc_control_down(dlist_t *, bot_t *);

bot_t *gfdpassc_off(dlist_t *, bot_t *);

int gfdpassc_input_client(gfdpassc_t *);
char *gfdpassc_change_string(bot_t *, char *, int);
char *gfdpassc_process_options(gfdpassc_t *, char *);
void gfdpassc_process_options_parse(gfdpassc_t *, char *);
void gfdpassc_process_options_parse_null(gfdpassc_t *, char *);

bot_t *gfdpassc_destroy_up_gfdpassc(gfdpassc_t *);
bot_t *gfdpassc_destroy_down_gfdpassc(gfdpassc_t *);

void gfdpassc_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void gfdpassc_free(void *);

bot_t *gfdpassc_input_get(gfdpassc_t *);

#endif
