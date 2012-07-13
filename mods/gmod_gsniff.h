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
#ifndef GMOD_GSNIFF_H
#define GMOD_GSNIFF_H

#include "bot.h"

#include "gmod_gfdpassc.h"

typedef struct gmod_gsniff {
	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;

	int fd;
	struct event ev;

	struct sockaddr_ll ll;
	socklen_t ll_len;

	char logfile[132];

} gsniff_t;

module_t gmod_gsniff_info;

void __gsniff_init__(void) __attribute__ ((constructor));

bot_t *gsniff_init(dlist_t *, bot_t *);
bot_t *gsniff_fini(dlist_t *, bot_t *);
bot_t *gsniff_help(dlist_t *, bot_t *);
bot_t *gsniff_run(dlist_t *, bot_t *);
bot_t *gsniff_control(dlist_t *, bot_t *, int, va_list);

bot_t *gsniff_input(dlist_t *, bot_t *);
bot_t *gsniff_output(dlist_t *, bot_t *);

bot_t *gsniff_destroy_up(dlist_t *, bot_t *);
bot_t *gsniff_destroy_down(dlist_t *, bot_t *);

bot_t *gsniff_control_up(dlist_t *, bot_t *);
bot_t *gsniff_control_down(dlist_t *, bot_t *);

bot_t *gsniff_off(dlist_t *, bot_t *);

bot_t *gsniff_input_initialize(gsniff_t *);
int gsniff_input_client(gsniff_t *);
char *gsniff_change_string(bot_t *, char *, int);
char *gsniff_process_options(gsniff_t *, char *);
void gsniff_process_options_parse(gsniff_t *, char *);
void gsniff_process_options_parse_logfile(gsniff_t *, char *);

bot_t *gsniff_destroy_up_gsniff(gsniff_t *);
bot_t *gsniff_destroy_down_gsniff(gsniff_t *);

void gsniff_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void gsniff_free(void *);

char *gsniff_show_proto(int, char *);
void gsniff_set_evhook(gsniff_t *);
void gsniff_evhook_sniff(int, short, void *);

void gsniff_log(gsniff_t *, char *);

struct udphdrx {
	__be16 source;
	__be16 dest;
	__be16 len;
	__sum16 check;
};

#endif
