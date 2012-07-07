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
#ifndef GMOD_GIRC_H
#define GMOD_GIRC_H

#include "bot.h"

bot_t *girc_init(dlist_t *, bot_t *);
bot_t *girc_fini(dlist_t *, bot_t *);
bot_t *girc_help(dlist_t *, bot_t *);
bot_t *girc_run(dlist_t *, bot_t *);

bot_t *girc_output(dlist_t *, bot_t *);
bot_t *girc_input(dlist_t *, bot_t *);

bot_t *girc_off(dlist_t *, bot_t *);
bot_t *girc_destroy_up(dlist_t *, bot_t *);
bot_t *girc_destroy_down(dlist_t *, bot_t *);

bot_t *girc_control_up(dlist_t *, bot_t *);
bot_t *girc_control_down(dlist_t *, bot_t *);

char *girc_change_string(bot_t *, char *, int);
void girc_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

typedef struct gmod_girc {
	bot_t *bot_root;
	bot_gmod_elm_t *gmod;
	dlist_t *dptr_gmod;

	int pong_count;
	int pong_count2;

	dlist_t *dl_channel;
	dlist_t *dl_nick;
	dlist_t *dl_nick_cur;

} girc_t;

char *girc_process_options(girc_t *, char *);
void girc_process_options_parse(girc_t *, char *);
void girc_process_options_parse_nick(girc_t *, char *);
void girc_process_options_parse_channel(girc_t *, char *);

int girc_send_nickinfo(girc_t *);
int girc_send_autojoins(girc_t *);
int girc_change_nick(girc_t *);
int girc_change_server(girc_t *);
int girc_line_info(girc_t *, char *line, int len);
int girc_send(girc_t *);
int girc_send_dptr(dlist_t *);
void girc_defaults_post(girc_t *);
void girc_destroy_gmod(bot_gmod_elm_t *);

void *girc_input_iohooks(void *, void *);

#endif
