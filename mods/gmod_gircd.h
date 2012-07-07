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
#ifndef GMOD_GIRCD_H
#define GMOD_GIRCD_H

#include "bot.h"

enum gmod_gircd_stuff {
	GRAW_CLIENT = 1,
	GRAW_SERVER,
	GRAW_RUN
};

/*
typedef struct gmod_gircd
{
} gmod_gircd_t;
*/

module_t gmod_gircd_info;

void __gircd_init__(void) __attribute__ ((constructor));

bot_t *gircd_init(dlist_t *, bot_t *);
bot_t *gircd_fini(dlist_t *, bot_t *);
bot_t *gircd_help(dlist_t *, bot_t *);
bot_t *gircd_run(dlist_t *, bot_t *);

bot_t *gircd_input(dlist_t *, bot_t *);
bot_t *gircd_output(dlist_t *, bot_t *);

bot_t *gircd_off(dlist_t *, bot_t *);

bot_t *gircd_destroy_down(dlist_t *, bot_t *);
bot_t *gircd_destroy_up(dlist_t *, bot_t *);

bot_t *gircd_control_down(dlist_t *, bot_t *);
bot_t *gircd_control_up(dlist_t *, bot_t *);

char *gircd_change_string(bot_t *, char *, int);

enum gircd_gmod_data_types {
	GIRCD_GMOD_DATA_TYPE_USER = 1,
	GIRCD_GMOD_DATA_TYPE_IRCD,
	GIRCD_GMOD_DATA_TYPE_BOT,
};

typedef struct gircd {
	bot_t *bot_root;
	bot_gmod_elm_t *gmod;
	dlist_t *dptr_gmod;
	char *name;
	dlist_t *users;
	dlist_t *users_unregistered;
	int cnt_users;
	dlist_t *chans;
	int cnt_chans;
	char *user_ronick;
	void *user_robot;
} gircd_t;

enum gircd_user_status_flags {
	GIRCD_USER_STATUS_UNREGISTERED = 0,
	GIRCD_USER_STATUS_REGISTERED
};

typedef struct gircd_user {
	int active;
	dlist_t *dptr_gircd_user;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;
	gircd_t *gircd_ptr;
	int status;
	bot_t *bot_sub;
	time_t time_logon;
	time_t time_ping;
	char *host_name;
	int host_port;
	char *nick;
	char *username;
	char *hostname;
	char *servername;
	char *realname;
	int userflags;
/* contains dptr's to channel's */
	dlist_t *chans;
	unsigned char tog_typed_quit;
} gircd_user_t;

typedef struct gircd_user_simple {
	char nick[32];
	char user[32];
	char host[132];
	time_t timestamp;
} gircd_user_simple_t;

typedef struct gircd_topic {
	char topic[255];
	gircd_user_simple_t author;
} gircd_topic_t;

typedef struct gircd_chan {
	char *name;
	char *key;
	int mode;
	gircd_topic_t topic;
	gircd_user_simple_t author;
/* contains dptr's to user's */
	dlist_t *users;
} gircd_chan_t;

void gircd_defaults_post(gircd_t *);
char *gircd_process_options(gircd_t *, char *);
void gircd_process_options_parse(gircd_t *, char *);
void gircd_process_options_parse_name(gircd_t *, char *);
void gircd_process_options_parse_ronick(gircd_t *, char *);
int gircd_link_down_dl(gircd_user_t *, dlist_t **);

bot_t *gircd_destroy_gmod(dlist_t *, bot_t *);
bot_t *gircd_destroy_gmod_user(gircd_user_t *);
bot_t *gircd_destroy_gmod_ircd(gircd_t *);

/*
bot_t *  gircd_destroy_up_bot(bot_t *);
bot_t *  gircd_destroy_down_bot(bot_t *);
*/

void gircd_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

gircd_user_t *gircd_user_init(gircd_t *, bot_t *, bot_t *);
dlist_t *gircd_chan_init(gircd_user_t *, char *);
void gircd_user_simple_init(gircd_user_simple_t *, gircd_user_t *);

/* sets */
void gircd_set_registered(gircd_user_t *);

/* checks */
int gircd_check_registered(gircd_user_t *);

/* responses */
int gircd_resp_init(dlist_t **, gircd_t *);
int gircd_resp_registered(dlist_t **, gircd_user_t *);
int gircd_resp_nick_inuse(dlist_t **, gircd_user_t *, char *);
int gircd_resp_nick_error(dlist_t **, gircd_user_t *, char *);
int gircd_resp_nick_changed(dlist_t **, gircd_user_t *, char *);
int gircd_resp_chan_illegal(dlist_t **, gircd_user_t *, char *);
int gircd_resp_chan_noexist(dlist_t **, gircd_user_t *, char *);
int gircd_resp_chan_join(dlist_t **, gircd_user_t *, gircd_chan_t *);
int gircd_resp_chan_part(dlist_t **, gircd_user_t *, gircd_chan_t *);
int gircd_resp_quit(dlist_t **, gircd_user_t *, char *);

/* parsing */
int gircd_parse_line(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_unregistered(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_nick(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_user(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_ping(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_quit(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_part(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_join(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_join_initial(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_privmsg(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_privmsg_channel(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_privmsg_personal(dlist_t **, char *, gircd_user_t *);
int gircd_parse_line_verb(dlist_t **, char *, gircd_user_t *);

/* finds etc */
dlist_t *gircd_find_user(gircd_t *, char *);
dlist_t *gircd_find_chan(gircd_t *, char *);
int gircd_find_chan_user_dptr(gircd_t *, gircd_chan_t *, dlist_t *);
int gircd_find_user_chan_dptr(gircd_t *, gircd_user_t *, dlist_t *);

/* user/chan routines */
int gircd_chan_add_user(gircd_t *, gircd_chan_t *, dlist_t *, gircd_user_t *,
			dlist_t *);
int gircd_chan_del_user(gircd_t *, gircd_chan_t *, dlist_t *, gircd_user_t *,
			dlist_t *);
char *gircd_chan_get_users(gircd_chan_t *);
int gircd_chan_tell(dlist_t **, gircd_chan_t *, gircd_user_t *);
int gircd_chans_tell(dlist_t **, gircd_user_t *);
int gircd_del_user(gircd_t *, gircd_user_t *, dlist_t *);

/* special stuff */
int gircd_special_chan_pm_console(dlist_t **, char *, gircd_user_t *);
gircd_user_t *gircd_special_user_init(gircd_t *, char *);

#endif
