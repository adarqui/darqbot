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
#ifndef GMOD_GRELINKC_H
#define GMOD_GRELINKC_H

#include "bot.h"

typedef struct gmod_grelinkc {
	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;

	int id;

	char *line;

} grelinkc_t;

module_t gmod_grelinkc_info;

void __grelinkc_init__(void) __attribute__ ((constructor));

bot_t *grelinkc_init(dlist_t *, bot_t *);
bot_t *grelinkc_fini(dlist_t *, bot_t *);
bot_t *grelinkc_help(dlist_t *, bot_t *);
bot_t *grelinkc_run(dlist_t *, bot_t *);
bot_t *grelinkc_control(dlist_t *, bot_t *, int, va_list);

bot_t *grelinkc_input(dlist_t *, bot_t *);
bot_t *grelinkc_output(dlist_t *, bot_t *);

bot_t *grelinkc_destroy_up(dlist_t *, bot_t *);
bot_t *grelinkc_destroy_down(dlist_t *, bot_t *);

bot_t *grelinkc_control_up(dlist_t *, bot_t *);
bot_t *grelinkc_control_down(dlist_t *, bot_t *);

bot_t *grelinkc_off(dlist_t *, bot_t *);

int grelinkc_input_client(grelinkc_t *);
char *grelinkc_change_string(bot_t *, char *, int);
char *grelinkc_process_options(grelinkc_t *, char *);
void grelinkc_process_options_parse(grelinkc_t *, char *);
void grelinkc_process_options_parse_line(grelinkc_t *, char *);
void grelinkc_process_options_parse_id(grelinkc_t *, char *);

bot_t *grelinkc_destroy_up_grelinkc(grelinkc_t *);
bot_t *grelinkc_destroy_down_grelinkc(grelinkc_t *);

void grelinkc_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void grelinkc_free(void *);

int grelinkc_op_nop(grelinkc_t *, relink_control_pkt_t *);
int grelinkc_op_relink(grelinkc_t *, relink_control_pkt_t *);
int grelinkc_op_hello(grelinkc_t *, relink_control_pkt_t *);
int grelinkc_op_result(grelinkc_t *, relink_control_pkt_t *);
int grelinkc_op_data_notify(grelinkc_t *);
int grelinkc_op_data(grelinkc_t *, relink_control_pkt_t *);

#endif
