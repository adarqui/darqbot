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
#ifndef GMOD_GRELINKD_H
#define GMOD_GRELINKD_H

#include "bot.h"

dlist_t *dl_relinkd_links;

typedef struct grelinkd_link {

	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;
	int isrelinked;

	int id;
	int type;
	int subtype;

	char *link_host;
	int link_port;

/*
  struct sockaddr_in link_sin;
  int link_fd;
  struct event link_ev;
*/

/*
struct sockaddr_in relinkc_sin;
int relinkc_fd;
struct event relinkc_ev;
*/
	int bot_id;

} grelinkd_t;

module_t gmod_grelinkd_info;

void __grelinkd_init__(void) __attribute__ ((constructor));

bot_t *grelinkd_init(dlist_t *, bot_t *);
bot_t *grelinkd_fini(dlist_t *, bot_t *);
bot_t *grelinkd_help(dlist_t *, bot_t *);
bot_t *grelinkd_run(dlist_t *, bot_t *);
bot_t *grelinkd_control(dlist_t *, bot_t *, int, va_list);

bot_t *grelinkd_input(dlist_t *, bot_t *);
bot_t *grelinkd_output(dlist_t *, bot_t *);

bot_t *grelinkd_destroy_up(dlist_t *, bot_t *);
bot_t *grelinkd_destroy_down(dlist_t *, bot_t *);

bot_t *grelinkd_control_up(dlist_t *, bot_t *);
bot_t *grelinkd_control_down(dlist_t *, bot_t *);

bot_t *grelinkd_off(dlist_t *, bot_t *);

int grelinkd_input_client(grelinkd_t *);
char *grelinkd_change_string(bot_t *, char *, int);
char *grelinkd_process_options(grelinkd_t *, char *);
void grelinkd_process_options_parse(grelinkd_t *, char *);
void grelinkd_process_options_parse_null(grelinkd_t *, char *);

bot_t *grelinkd_destroy_up_grelinkd(grelinkd_t *);
bot_t *grelinkd_destroy_down_grelinkd(grelinkd_t *);

void grelinkd_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void grelinkd_free(void *);

bot_t *grelinkd_input_uninitialized(grelinkd_t *);

int grelinkd_op_nop(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_relink(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_relink_notify(grelinkd_t *);
int grelinkd_op_destroy(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_destroy_link(grelinkd_t *, int);
int grelinkd_op_destroy_sub(grelinkd_t *, int);

int grelinkd_op_reload(grelinkd_t *);
int grelinkd_op_query(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_hello(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_result(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_data(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_data_notify(grelinkd_t *);

/*
int grelinkd_subtype_process (grelinkd_link_t *, char *, int);
int grelinkd_subtype_process_irc (grelinkd_link_t *, char *, int);
*/

int grelinkd_op_query_links(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_query_subs(grelinkd_t *, relink_control_pkt_t *);
int grelinkd_op_query_reload(grelinkd_t *, relink_control_pkt_t *);

dlist_t *grelinkd_find_id(grelinkd_t *, bot_t **);
void grelinkd_combine_dlist(grelinkd_t *, grelinkd_t *);

#endif
