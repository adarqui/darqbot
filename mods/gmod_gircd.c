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
#include "gmod_gircd.h"

void __gircd_init__(void) __attribute__ ((constructor));

void __gircd_init__(void)
{

	strlcpy_buf(gmod_gircd_info.name, "gmod_gircd");
	strlcpy_buf(gmod_gircd_info.trigger, "^gircd");

	gmod_gircd_info.init = gircd_init;
	gmod_gircd_info.fini = gircd_fini;
	gmod_gircd_info.help = gircd_help;
	gmod_gircd_info.run = gircd_run;
	gmod_gircd_info.control = NULL;

	gmod_gircd_info.destroy_up = gircd_destroy_up;
	gmod_gircd_info.destroy_down = gircd_destroy_down;

	gmod_gircd_info.control_up = gircd_control_up;
	gmod_gircd_info.control_down = gircd_control_down;

	gmod_gircd_info.off = gircd_off;

	gmod_gircd_info.output = gircd_output;
	gmod_gircd_info.input = gircd_input;

	gmod_gircd_info.self = &gmod_gircd_info;

	debug(NULL, "__gircd_init__: Loaded gmod_gircd\n");

	return;
}

bot_t *gircd_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gircd_init: Entered\n");
	return NULL;
}

bot_t *gircd_fini(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "gircd_fini: Entered\n");
	return NULL;
}

bot_t *gircd_off(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "gircd_off: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gircd_destroy_gmod(dlist_node, bot);

	return NULL;
}

bot_t *gircd_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gircd_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gircd";

	return NULL;
}

bot_t *gircd_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod_parent = NULL, *dptr_gmod = NULL;
	gircd_t *gircd = NULL;
	bot_gmod_elm_t *gmod = NULL, *gmod_parent = NULL;
	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gircd_run: Entered, %p %p\n", dlist_node, bot);

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gircd_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gircd_info.trigger,
					       gmod_gircd_info.trigger_ext);
	if (!dptr_gmod) {
		debug(NULL, "gircd_run: GIRCD DPTR=NULL\n");
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gircd_run: GIRCD GMOD = NULL\n");
		return NULL;
	}

	dptr_gmod_parent =
	    (dlist_t *) gmodule_find_gmod_dptr(bot->parent,
					       gmod_gircd_info.trigger,
					       gmod_gircd_info.trigger_ext);
	if (!dptr_gmod_parent) {
		debug(NULL, "gircd_run: GIRCD DPTR=NULL\n");
		return NULL;
	}

	gmod_parent = (bot_gmod_elm_t *) dlist_data(dptr_gmod_parent);
	if (!gmod_parent) {
		debug(NULL, "gircd_run: GIRCD GMOD=NULL\n");
		return NULL;
	}

	if (!gmod_parent->data) {
		debug(NULL, "gircd_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
		gircd_gmod_init(bot->parent, gmod_parent, dptr_gmod_parent);
		gircd = gmod_parent->data;
	} else
		gircd = gmod_parent->data;

	MOD_OPTIONS_TOP_HALF;
	gircd_process_options(gircd, dl_options_ptr);
	MOD_OPTIONS_BOTTOM_HALF;

/* add robot */
	gircd_special_user_init(gircd, gircd->user_ronick);
	gircd_defaults_post(gircd);

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);
	gircd_input(dptr_gmod, bot);
	gmodule_set_has_run(gmod);

	return bot;
}

char *gircd_process_options(gircd_t * gircd, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];
	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gircd_process_options: Entered\n");

	if (!gircd || !sNULL(string))
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	dl = tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		      "...");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		gircd_process_options_parse(gircd, (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gircd_process_options_parse(gircd_t * gircd, char *string)
{

	if (!gircd || !sNULL(string))
		return;

	debug(NULL, "gircd_process_options_parse: Entered\n");

	if (!strncasecmp_len(string, "name=")) {
		gircd_process_options_parse_name(gircd,
						 &string[strlen("name=")]);
	} else if (!strncasecmp_len(string, "ronick=")) {
		gircd_process_options_parse_ronick(gircd,
						   &string[strlen("ronick=")]);
	}

	gmodule_parse_common_options(gircd->bot_root, gircd->gmod, string);

	return;
}

void gircd_process_options_parse_name(gircd_t * gircd, char *string)
{

	if (!gircd || !sNULL(string))
		return;

	strstrip_chars(string, " \r\n\t");

	if (gircd->name)
		free(gircd->name);

	gircd->name = strdup(string);

	return;
}

void gircd_process_options_parse_ronick(gircd_t * gircd, char *string)
{

	if (!gircd || !sNULL(string))
		return;

	strstrip_chars(string, " \r\n\t");

	if (gircd->user_ronick)
		free(gircd->user_ronick);

	gircd->user_ronick = strdup(string);

	return;
}

char *gircd_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char *sep_ptr;

	if (!bot || !sNULL(string))
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	return str;
}

int gircd_link_down_dl(gircd_user_t * gircd_user, dlist_t ** dl_text)
{
	dlist_t *dptr_text = NULL;
	char *str_ptr = NULL;
	debug(NULL, "gircd_link_down_dl: Entered\n");

	int ntot = 0;

	if (!gircd_user || !dl_text)
		return -1;

	dlist_fornext(*(dl_text), dptr_text) {
		str_ptr = (char *)dlist_data(dptr_text);

		memcopy(gircd_user->bot_sub->txt_data_out, str_ptr,
			sizeof(gircd_user->bot_sub->txt_data_out) - 1,
			strlen(str_ptr) + 1);

		gircd_user->bot_sub->txt_data_out_sz =
		    strlen(gircd_user->bot_sub->txt_data_out);

		ntot += gircd_user->bot_sub->txt_data_out_sz;
		gmodule_down(gircd_user->dptr_gmod, gircd_user->bot_sub);
	}

	memset(gircd_user->bot_sub->txt_data_out, 0,
	       sizeof(gircd_user->bot_sub->txt_data_out));
	gircd_user->bot_sub->txt_data_out_sz = 0;

	return ntot;
}

bot_t *gircd_output(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "gircd_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	return bot;
}

bot_t *gircd_input(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dl_text, *dl, *dptr;

	int n;
	char buf[MAX_BUF_SZ + 1];

	bot_gmod_elm_t *gmod = NULL, *gmod_parent = NULL;
	gircd_t *gircd = NULL;
	gircd_user_t *gircd_user = NULL;
	dlist_t *dptr_gircd_user = NULL;

	char *line = NULL;

	debug(bot, "gircd_input: Entered:\n");

	if (!bot || !dlist_node)
		return NULL;

	if (!bot->parent) {
		return NULL;
	}

	gmod_parent =
	    (bot_gmod_elm_t *) gmodule_find_gmod(bot->parent,
						 gmod_gircd_info.trigger,
						 gmod_gircd_info.trigger_ext);
	if (!gmod_parent) {
		return NULL;
	}

	gircd = (gircd_t *) gmod_parent->data;
	if (!gircd) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	if (!gmod->data) {
/* NEW CONNECTION */
		debug(NULL,
		      "gircd_input: GMOD->DATA=NULL, NEW UNREGISTERED USER");
		gircd_user = gircd_user_init(gircd, bot->parent, bot);

		dl_text = NULL;
		n = gircd_resp_init(&dl_text, gircd);
		gircd_link_down_dl(gircd_user, &dl_text);
		dl_str_destroy(&dl_text);
		return bot;
	}

	dptr_gircd_user = (dlist_t *) gmod->data;
	if (!dptr_gircd_user) {
/* FIX: destroy */
		return NULL;
	}
	gircd_user = (gircd_user_t *) dlist_data(dptr_gircd_user);

	debug(NULL, "gircd_input: gmod=%p, dptr_gircd_user=%p, gircd_user=%p\n",
	      gmod, dptr_gircd_user, gircd_user);

	memset(buf, 0, sizeof(buf));
	memcopy(buf, bot->txt_data_in, sizeof(buf) - 1, bot->txt_data_in_sz);

	n = bot->txt_data_in_sz;
	buf[n] = '\0';

	dl = tokenize(bot, buf,
		      TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES |
		      TOKENIZE_MATCHANY, "\r\n");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		line = (char *)dlist_data(dptr);

		dl_text = NULL;

		if (gircd_user->status == GIRCD_USER_STATUS_UNREGISTERED) {
/* unregistered user, attempt to register */
			gircd_parse_line_unregistered(&dl_text, line,
						      gircd_user);

			n = gircd_check_registered(gircd_user);
			if (n) {
				gircd_set_registered(gircd_user);

				dl_text = NULL;
				n = gircd_resp_registered(&dl_text, gircd_user);
				if (dl_text) {
					gircd_link_down_dl(gircd_user,
							   &dl_text);
					dl_str_destroy(&dl_text);
				}
			}

		} else if (gircd_user->status == GIRCD_USER_STATUS_REGISTERED) {
			gircd_parse_line(&dl_text, line, gircd_user);
		} else
			continue;

		if (gircd_user->active == 0) {
			gircd_destroy_down(dlist_node, bot);
			return NULL;
		}

		if (dl_text) {
			gircd_link_down_dl(gircd_user, &dl_text);
			dl_str_destroy(&dl_text);
		}

	}

	if (dl)
		tokenize_destroy(NULL, &dl);

	return bot;
}

void gircd_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gircd = (gircd_t *) calloc(1, sizeof(gircd_t));

	if (!gircd)
		return;

	gircd->gmod = gmod;
	gircd->dptr_gmod = dptr_gmod;

	gmod->data_type = GIRCD_GMOD_DATA_TYPE_IRCD;
	gmod->data = gircd;

	gircd->bot_root = bot;

// FIXFIX
	gircd->name = strdup("dircd");

	return;
}

void
gircd_user_simple_init(gircd_user_simple_t * gircd_user_simple,
		       gircd_user_t * gircd_user)
{

	debug(NULL, "gircd_user_simple_init: %p %p\n", gircd_user_simple,
	      gircd_user);

	if (!gircd_user_simple || !gircd_user)
		return;

	strlcpy_buf(gircd_user_simple->nick, gircd_user->nick);
	strlcpy_buf(gircd_user_simple->user, gircd_user->username);
	if (gircd_user->host_name) {
		strlcpy_buf(gircd_user_simple->host, gircd_user->host_name);
	} else {
		strlcpy_buf(gircd_user_simple->host, "unknown.com");
	}

	time(&gircd_user_simple->timestamp);

	return;
}

gircd_user_t *gircd_user_init(gircd_t * gircd, bot_t * bot, bot_t * bot_sub)
{
	dlist_t *dptr_gmod = NULL;
	dlist_t *dptr_user = NULL;
	gircd_user_t *gircd_user = NULL;
	bot_gmod_elm_t *gmod_sub = NULL;
	debug(NULL, "gircd_user_init: Entered\n");
	if (!gircd || !bot || !bot_sub)
		return NULL;
	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot_sub, gmod_gircd_info.trigger,
					       gmod_gircd_info.trigger_ext);
	if (!dptr_gmod) {
		debug(NULL,
		      "gircd_user_init: GIRCD USER INIT DPTR_GMOD = NULL");
		return NULL;
	}
	gmod_sub = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod_sub) {
		debug(NULL, "gircd_user_init: GIRCD USER INIT GMOD_SUB = NULL");
		return NULL;
	}
	gircd_user = (gircd_user_t *) calloc(1, sizeof(gircd_user_t));
	if (!gircd_user)
		return NULL;
	gircd_user->active = 1;
	gircd_user->gmod = gmod_sub;
	gircd_user->dptr_gmod = dptr_gmod;
	gircd_user->gircd_ptr = gircd;
	gircd_user->status = GIRCD_USER_STATUS_UNREGISTERED;
	gircd_user->bot_sub = bot_sub;
	time(&gircd_user->time_logon);
	gircd_user->host_name =
	    fd_link_get_latest_host(gircd_user->bot_sub,
				    gmod_gircd_info.trigger,
				    gmod_gircd_info.trigger_ext);

	if (!sNULL(gircd_user->host_name))
		gircd_user->host_name = strdup("unknown.com");

	dptr_user = dlist_Dinsert_after(&gircd->users_unregistered, gircd_user);

/* bot_sub's gmod gets a dlist_t, not the gircd_user, this way, can delete from unregistered very fast, and insert into registered users very fast */
	gmod_sub->data_type = GIRCD_GMOD_DATA_TYPE_USER;
	gmod_sub->data = (dlist_t *) dptr_user;

	gircd_user->dptr_gircd_user = dptr_user;

	return gircd_user;
}

dlist_t *gircd_chan_init(gircd_user_t * gircd_user, char *name)
{
	dlist_t *dptr_chan = NULL;
	gircd_chan_t *gircd_chan = NULL;
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_chan_init: Entered: %p %s\n", gircd_user, name);

	if (!gircd_user || !sNULL(name))
		return NULL;

	gircd = gircd_user->gircd_ptr;

	gircd_chan = (gircd_chan_t *) calloc(1, sizeof(gircd_chan_t));
	if (!gircd_chan)
		return NULL;

	gircd_chan->name = strdup(name);
	gircd_user_simple_init(&gircd_chan->author, gircd_user);

	dptr_chan = dlist_Dinsert_after(&gircd->chans, gircd_chan);

	return dptr_chan;
}

void gircd_defaults_post(gircd_t * gircd)
{

	if (!gircd)
		return;

	if (!sNULL(gircd->name))
		gircd->name = strdup("dircd");

	if (!sNULL(gircd->user_ronick))
		gircd->user_ronick = strdup("darqbot");

	return;
}

/* 
 * SETS *
 */
void gircd_set_registered(gircd_user_t * gircd_user)
{
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_set_registered: Entered\n");

	if (!gircd_user)
		return;

	gircd = gircd_user->gircd_ptr;

	if (gircd_user->status == GIRCD_USER_STATUS_REGISTERED) {
		debug(NULL, "gircd_set_registered: ALREADY REGISTERED\n");
		return;
	}

	gircd_user->status = GIRCD_USER_STATUS_REGISTERED;

	dlist_remove(&gircd->users_unregistered, gircd_user->dptr_gircd_user);
	dlist_insert_after(&gircd->users, NULL, gircd_user->dptr_gircd_user);

	debug(NULL, "gircd_set_registered: ADDED REGISTERED USER\n");

	return;
}

/*
 * CHECKS
 */
int gircd_check_registered(gircd_user_t * gircd_user)
{

	debug(NULL, "gircd_check_registered: Entered: %p %p %p %p %p\n",
	      gircd_user->nick, gircd_user->username, gircd_user->hostname,
	      gircd_user->servername, gircd_user->realname);

	if (!gircd_user)
		return 0;

	if (gircd_user->nick && gircd_user->username && gircd_user->hostname
	    && gircd_user->servername && gircd_user->realname)
		return 1;

	return 0;
}

/* 
 * RESPONSES
 */

int gircd_resp_init(dlist_t ** dl_text, gircd_t * gircd)
{

	if (!dl_text || !gircd)
		return -1;

	dl_str_unite(dl_text, ":%s NOTICE AUTH :*** Found your hostname.\n",
		     gircd->name);
	dl_str_unite(dl_text, ":%s NOTICE AUTH :*** Checking Ident.\n",
		     gircd->name);

	return 1;
}

int gircd_resp_registered(dlist_t ** dl_text, gircd_user_t * gircd_user)
{
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_resp_registered: Entered\n");

	if (!dl_text || !gircd_user)
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s 001 %s :Welcome to %s IRC %s\n", gircd->name,
		     gircd_user->nick, gircd->name, gircd_user->nick);
	dl_str_unite(dl_text, ":%s 002 %s :Your host is %s:%i\n", gircd->name,
		     gircd_user->nick, gircd_user->host_name,
		     gircd_user->host_port);
	dl_str_unite(dl_text, ":%s 003 %s :This server ..\n", gircd->name,
		     gircd_user->nick);
	dl_str_unite(dl_text, ":%s!~%s@%s MODE %s :+i\n", gircd_user->nick,
		     gircd_user->username, gircd_user->host_name,
		     gircd_user->nick);

	return 1;
}

int
gircd_resp_nick_inuse(dlist_t ** dl_text, gircd_user_t * gircd_user, char *nick)
{
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_resp_nick_inuse: Entered\n");

	if (!gircd_user || !sNULL(nick))
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s %i %s %s :Nickname is already in use.\n",
		     gircd->name, IRC_NICK_INUSE,
		     gircd_user->nick != NULL ? gircd_user->nick : "*", nick);

	return 1;
}

int
gircd_resp_nick_error(dlist_t ** dl_text, gircd_user_t * gircd_user, char *nick)
{
	gircd_t *gircd = NULL;
	debug(NULL, "gircd_resp_nick_error: Entered\n");

	if (!dl_text || !gircd_user || !sNULL(nick))
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s %i %s %s :Erroneous nickname.\n",
		     gircd->name, IRC_NICK_ERROR,
		     gircd_user->nick != NULL ? gircd_user->nick : "*",
		     strlen(nick) == 0 ? "NULL" : nick);

	return 1;
}

int
gircd_resp_nick_changed(dlist_t ** dl_text, gircd_user_t * gircd_user,
			char *new_nick)
{
	gircd_t *gircd = NULL;
	debug(NULL, "gircd_resp_nick_changed: Entered\n");

	if (!dl_text || !gircd_user || !sNULL(new_nick))
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s!~%s@%s NICK :%s\n",
		     gircd_user->nick, gircd_user->username,
		     gircd_user->host_name, new_nick);

	gircd_chans_tell(dl_text, gircd_user);

	return 1;
}

int
gircd_resp_chan_illegal(dlist_t ** dl_text, gircd_user_t * gircd_user,
			char *chan)
{

	gircd_t *gircd = NULL;
	debug(NULL, "gircd_resp_chan_illegal: Entered\n");

	if (!dl_text || !gircd_user || !sNULL(chan))
		return -1;
	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s %i %s %s :Illegal channel name.\n",
		     gircd->name, IRC_CHANNEL_ILLEGAL,
		     gircd_user->nick != NULL ? gircd_user->nick : "*",
		     strlen(chan) == 0 ? "NULL" : chan);

	return 1;
}

int
gircd_resp_chan_noexist(dlist_t ** dl_text, gircd_user_t * gircd_user,
			char *chan)
{
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_resp_chan_noexist: Entered\n");

	if (!dl_text || !gircd_user || !sNULL(chan))
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s %i %s %s :No such nick/channel.\n",
		     gircd->name, IRC_CHANNEL_NOEXIST,
		     gircd_user->nick != NULL ? gircd_user->nick : "*",
		     strlen(chan) == 0 ? "NULL" : chan);

	return 1;
}

int
gircd_resp_chan_join_initial(dlist_t ** dl_text, gircd_user_t * gircd_user,
			     gircd_chan_t * gircd_chan)
{
	gircd_t *gircd = NULL;
	char *str_users = NULL;

	debug(NULL, "gircd_resp_chan_join: Entered\n");
	if (!dl_text || !gircd_user || !gircd_chan)
		return -1;

	gircd = gircd_user->gircd_ptr;

	str_users = gircd_chan_get_users(gircd_chan);
	if (!sNULL(str_users))
		return -1;

	dl_str_unite(dl_text, ":%s %i %s = %s :%s\r\n", gircd->name,
		     IRC_CHANNEL_USERS, gircd_user->nick, gircd_chan->name,
		     str_users);

	free(str_users);

	dl_str_unite(dl_text, ":%s %i %s %s :End of /names list\r\n",
		     gircd->name, IRC_NAMELIST_END, gircd_user->nick,
		     gircd_chan->name);

	return 1;
}

int
gircd_resp_chan_join(dlist_t ** dl_text, gircd_user_t * gircd_user,
		     gircd_chan_t * gircd_chan)
{
	gircd_t *gircd = NULL;
	dlist_t *dl_text_new = NULL;
	char *str_users = NULL;
	int n;

	debug(NULL, "gircd_resp_chan_join: Entered\n");
	if (!dl_text || !gircd_user || !gircd_chan)
		return -1;

	gircd = gircd_user->gircd_ptr;

	str_users = gircd_chan_get_users(gircd_chan);
	if (!sNULL(str_users))
		return -1;

	dl_str_unite(&dl_text_new, ":%s!~%s@%s JOIN :%s\r\n", gircd_user->nick,
		     gircd_user->username, gircd_user->host_name,
		     gircd_chan->name);

	n = gircd_chan_tell(&dl_text_new, gircd_chan, NULL);

	return 1;
}

int
gircd_resp_chan_part(dlist_t ** dl_text, gircd_user_t * gircd_user,
		     gircd_chan_t * gircd_chan)
{
	gircd_t *gircd = NULL;
	int n;

	debug(NULL, "gircd_resp_chan_part: Entered\n");
	if (!dl_text || !gircd_user || !gircd_chan)
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s!~%s@%s PART :%s\r\n", gircd_user->nick,
		     gircd_user->username, gircd_user->host_name,
		     gircd_chan->name);

	n = gircd_chan_tell(dl_text, gircd_chan, NULL);

	return 1;
}

int gircd_resp_quit(dlist_t ** dl_text, gircd_user_t * gircd_user, char *msg)
{
	gircd_t *gircd = NULL;
	int n;

	debug(NULL, "gircd_resp_chan_part: Entered\n");
	if (!dl_text || !gircd_user || !sNULL(msg))
		return -1;

	gircd = gircd_user->gircd_ptr;

	dl_str_unite(dl_text, ":%s!~%s@%s QUIT :%s\r\n", gircd_user->nick,
		     gircd_user->username, gircd_user->host_name, msg);

	n = gircd_chans_tell(dl_text, gircd_user);

	return 1;
}

/* 
 *PARSING
 */

int gircd_parse_line(dlist_t ** dl_text, char *line, gircd_user_t * gircd_user)
{
	char *str_rest;
	debug(NULL, "gircd_parse_line: Entered\n");

	if (!dl_text || !sNULL(line) || !gircd_user) {
		return -1;
	}
// FIXFIX fix these to, get rid of bad chars, check to see if nicks/chans exist, etc.. tuff like that */
	if (!strncasecmp_len(line, "NICK ")) {
		str_rest = &line[strlen("NICK ")];
		gircd_parse_line_nick(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "USER ")) {
		str_rest = &line[strlen("USER ")];
		gircd_parse_line_user(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "PING ")) {
		str_rest = &line[strlen("PING ")];
		gircd_parse_line_ping(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "JOIN ")) {
		str_rest = &line[strlen("JOIN ")];
		gircd_parse_line_join(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "PART ")) {
		str_rest = &line[strlen("PART ")];
		gircd_parse_line_part(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "QUIT ")) {
		str_rest = &line[strlen("QUIT ")];
		gircd_parse_line_quit(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "PRIVMSG ")) {
		str_rest = &line[strlen("PRIVMSG ")];
		gircd_parse_line_privmsg(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "VERB ")) {
		str_rest = &line[strlen("VERB ")];
		gircd_parse_line_verb(dl_text, str_rest, gircd_user);
	}

	return 1;
}

int
gircd_parse_line_unregistered(dlist_t ** dl_text, char *line,
			      gircd_user_t * gircd_user)
{
	char *str_rest;
	debug(NULL, "gircd_parse_line_unregistered: Entered\n");

	if (!dl_text || !sNULL(line) || !gircd_user) {
		return -1;
	}
// FIXFIX fix these to, get rid of bad chars, check to see if nicks/chans exist, etc.. tuff like that */
	if (!strncasecmp_len(line, "NICK ")) {
		str_rest = &line[strlen("NICK ")];
		gircd_parse_line_nick(dl_text, str_rest, gircd_user);
	} else if (!strncasecmp_len(line, "USER ")) {
		str_rest = &line[strlen("USER ")];
		gircd_parse_line_user(dl_text, str_rest, gircd_user);
	}

	return 1;
}

int
gircd_parse_line_verb(dlist_t ** dl_text, char *string,
		      gircd_user_t * gircd_user)
{

	bot_t *bot;
	dlist_t *dptr_a = NULL, *dptr_b = NULL, *dptr_c = NULL;
	gircd_t *gircd = NULL;
	gircd_user_t *gircd_user_ptr = NULL;
	gircd_chan_t *gircd_chan_ptr = NULL;
	char *privmsg_string;

	debug(NULL, "gircd_parse_line_verb: Entered\n");

	if (!dl_text || !sNULL(string) || !gircd_user) {
		return -1;
	}

	gircd = gircd_user->gircd_ptr;
	bot = gircd_user->bot_sub;

	privmsg_string = str_unite(":darq!darq@darq PRIVMSG %s", string);

	dl_str_unite(dl_text, "%s :name=[%s]\n", privmsg_string, gircd->name);
	dl_str_unite(dl_text,
		     "%s : bot_root[%p] users[%i] users_unregistered[%i]\n",
		     privmsg_string, gircd->bot_root, dlist_size(gircd->users),
		     dlist_size(gircd->users_unregistered));

	dl_str_unite(dl_text, "%s : USERS:\n", privmsg_string);

	dlist_fornext(gircd->users, dptr_a) {
		gircd_user_ptr = (gircd_user_t *) dlist_data(dptr_a);

		dl_str_unite(dl_text,
			     "%s :  nick=[%s] username=[%s] realname=[%s]\n",
			     privmsg_string, gircd_user_ptr->nick,
			     gircd_user_ptr->username,
			     gircd_user_ptr->realname);

		dl_str_unite(dl_text,
			     "%s :   active=[%i] gircd_ptr=[%p] status=[%i] bot_sub=[%p] time_logon=[%li] time_ping=[%li]\n",
			     privmsg_string, gircd_user_ptr->active,
			     gircd_user_ptr->gircd_ptr, gircd_user_ptr->status,
			     gircd_user_ptr->bot_sub,
			     gircd_user_ptr->time_logon,
			     gircd_user_ptr->time_ping);

		dl_str_unite(dl_text,
			     "%s :   host_name=[%s] host_port=[%i] userflags=[%i] chans=[%i]\n",
			     privmsg_string, gircd_user_ptr->host_name,
			     gircd_user_ptr->host_port,
			     gircd_user_ptr->userflags,
			     dlist_size(gircd_user_ptr->chans));

		dlist_fornext(gircd_user_ptr->chans, dptr_b) {
			dptr_c = (dlist_t *) dlist_data(dptr_b);
			if (!dptr_c)
				continue;
			gircd_chan_ptr = (gircd_chan_t *) dlist_data(dptr_c);

			dl_str_unite(dl_text, "%s :    chan=[%s : %p]\n",
				     privmsg_string, gircd_chan_ptr->name,
				     dptr_c);
		}

	}

	dl_str_unite(dl_text, "%s : CHANNELS:\n", privmsg_string);

	dlist_fornext(gircd->chans, dptr_a) {
		gircd_chan_ptr = (gircd_chan_t *) dlist_data(dptr_a);

		dl_str_unite(dl_text,
			     "%s :  name=[%s] key=[%s] mode=[%i] users=[%i]\n",
			     privmsg_string, gircd_chan_ptr->name,
			     gircd_chan_ptr->key, gircd_chan_ptr->mode,
			     dlist_size(gircd_chan_ptr->users));

		dlist_fornext(gircd_chan_ptr->users, dptr_b) {
			dptr_c = (dlist_t *) dlist_data(dptr_b);
			if (!dptr_c)
				continue;

			gircd_user_ptr = (gircd_user_t *) dlist_data(dptr_c);

			dl_str_unite(dl_text, "%s :   user=[%s : %p]\n",
				     privmsg_string, gircd_user_ptr->nick,
				     dptr_c);
		}

	}

	free(privmsg_string);

	return 1;
}

int
gircd_parse_line_nick(dlist_t ** dl_text, char *nick, gircd_user_t * gircd_user)
{
	gircd_t *gircd = NULL;
	dlist_t *dptr = NULL;

	debug(NULL, "gircd_parse_line_nick: Entered\n");

	if (!dl_text || !sNULL(nick) || !gircd_user) {
		return -1;
	}

	gircd = gircd_user->gircd_ptr;

	strstrip_chars_fmt(nick, STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK,
			   isalnum, "^_.-", NULL);

	if (!sNULL(nick)) {
		gircd_resp_nick_error(dl_text, gircd_user, nick);
		return -1;
	}

	dptr = gircd_find_user(gircd, nick);
	if (dptr) {
		gircd_resp_nick_inuse(dl_text, gircd_user, nick);
		return -1;
	}

	gircd_resp_nick_changed(dl_text, gircd_user, nick);

	gircd_user->nick = strdup(nick);

	return 1;
}

int
gircd_parse_line_user(dlist_t ** dl_text, char *user_info,
		      gircd_user_t * gircd_user)
{
	char *tok_username = NULL;
	char *tok_hostname = NULL;
	char *tok_servername = NULL;
	char *tok_realname = NULL;

	debug(NULL, "gircd_parse_line_nick: Entered\n");

	if (!dl_text || !sNULL(user_info) || !gircd_user) {
		return -1;
	}

	tok_username = strtok(user_info, " ");
	if (!sNULL(tok_username))
		return -1;

	tok_hostname = strtok(NULL, " ");
	if (!sNULL(tok_hostname))
		return -1;

	tok_servername = strtok(NULL, " ");
	if (!sNULL(tok_servername))
		return -1;

	tok_realname = strtok(NULL, "");
	if (!sNULL(tok_realname))
		return -1;

	strstrip_chars_fmt(tok_username,
			   STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK, isalnum,
			   "^_.-", NULL);
	strstrip_chars_fmt(tok_hostname,
			   STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK, isalnum,
			   "^_.-", NULL);
	strstrip_chars_fmt(tok_servername,
			   STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK, isalnum,
			   "^_.-", NULL);
	strstrip_chars_fmt(tok_realname,
			   STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK, isalnum,
			   "^_.-", NULL);

	gircd_user->username = strdup(tok_username);
	gircd_user->hostname = strdup(tok_hostname);
	gircd_user->servername = strdup(tok_servername);
	gircd_user->realname = strdup(tok_realname);

	debug(NULL, "gircd_parse_line_user: set user: [%s] [%s] [%s] [%s]\n",
	      gircd_user->username, gircd_user->hostname,
	      gircd_user->servername, gircd_user->realname);

	return 1;
}

int
gircd_parse_line_ping(dlist_t ** dl_text, char *ping_resp,
		      gircd_user_t * gircd_user)
{

	debug(NULL, "gircd_parse_line_ping: Entered\n");

	if (!dl_text || !sNULL(ping_resp) || !gircd_user) {
		return -1;
	}

	strstrip_chars_fmt(ping_resp,
			   STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK, isalnum,
			   "^_.-", NULL);

	dl_str_unite(dl_text, "PONG %s\n", ping_resp);

	return 1;
}

int
gircd_parse_line_join(dlist_t ** dl_text, char *chan, gircd_user_t * gircd_user)
{
	dlist_t *dptr_chan = NULL;
	dlist_t *dptr_user = NULL;
	gircd_t *gircd = NULL;
	gircd_chan_t *gircd_chan = NULL;

	debug(NULL, "gircd_parse_line_join: Entered\n");

	if (!dl_text || !sNULL(chan) || !gircd_user) {
		return -1;
	}

	gircd = gircd_user->gircd_ptr;

	strstrip_chars_fmt(chan, STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK,
			   '#', isalnum, ispunct, NULL);

	if (chan[0] != '#') {
		gircd_resp_chan_illegal(dl_text, gircd_user, chan);
		return -1;
	}

	dptr_user = gircd_find_user(gircd, gircd_user->nick);
	if (!dptr_user) {
/* doesn't exist, return error */
		return -1;
	}

	dptr_chan = gircd_find_chan(gircd, chan);
	if (!dptr_chan) {
/* doesn't exist, create it */
		dptr_chan = gircd_chan_init(gircd_user, chan);
	}

	if (!dptr_chan) {
		gircd_resp_chan_illegal(dl_text, gircd_user, chan);
		return -1;
	}

	if (gircd_find_user_chan_dptr(gircd, gircd_user, dptr_chan)) {
/* we're already in this channel */
		return -1;
	}

	gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan);

	gircd_chan_add_user(gircd, gircd_chan, dptr_chan, gircd_user,
			    dptr_user);

	gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan);
	gircd_resp_chan_join_initial(dl_text, gircd_user, gircd_chan);

	gircd_resp_chan_join(dl_text, gircd_user, gircd_chan);

	return 1;
}

int
gircd_parse_line_part(dlist_t ** dl_text, char *chan, gircd_user_t * gircd_user)
{
	dlist_t *dptr_chan = NULL;
	dlist_t *dptr_user = NULL;
	gircd_t *gircd = NULL;
	gircd_chan_t *gircd_chan = NULL;
	int n;

	debug(NULL, "gircd_parse_line_part: Entered\n");

	if (!dl_text || !sNULL(chan) || !gircd_user) {
		return -1;
	}

	strstrip_chars_fmt(chan, STRSTRIP_CHARS_KEEP | STRSTRIP_CHARS_SHRINK,
			   '#', isalnum, ispunct, NULL);

	gircd = gircd_user->gircd_ptr;

	if (chan[0] != '#') {
		gircd_resp_chan_illegal(dl_text, gircd_user, chan);
		return -1;
	}

	dptr_user = gircd_user->dptr_gircd_user;

	dptr_chan = gircd_find_chan(gircd, chan);
	if (!dptr_chan) {
/* doesn't exist */
		return -1;
	}

	if (!gircd_find_user_chan_dptr(gircd, gircd_user, dptr_chan)) {
/* we're not in this channel */
		return -1;
	}

	gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan);
	n = gircd_chan_del_user(gircd, gircd_chan, dptr_chan, gircd_user,
				dptr_user);
	gircd_resp_chan_part(dl_text, gircd_user, gircd_chan);
	return 1;
}

int
gircd_parse_line_quit(dlist_t ** dl_text, char *msg, gircd_user_t * gircd_user)
{
	dlist_t *dptr_user = NULL;
	gircd_t *gircd = NULL;

	debug(NULL, "gircd_parse_line_quit: Entered\n");

	if (!dl_text || !sNULL(msg) || !gircd_user) {
		return -1;
	}

	gircd = gircd_user->gircd_ptr;

	dptr_user = gircd_find_user(gircd, gircd_user->nick);
	if (!dptr_user) {
/* doesn't exist, return error */
		return -1;
	}

	gircd_resp_quit(dl_text, gircd_user, msg);
	gircd_del_user(gircd, gircd_user, dptr_user);

	return 1;
}

int
gircd_parse_line_privmsg(dlist_t ** dl_text, char *chan,
			 gircd_user_t * gircd_user)
{

	debug(NULL, "gircd_parse_line_privmsg: Entered\n");

	if (!dl_text || !sNULL(chan) || !gircd_user) {
		return -1;
	}

	if (chan[0] == '#') {
/* msg a channel */

/* special channels */
		if (!strncasecmp_len(chan, "#console ")) {
			return gircd_special_chan_pm_console(dl_text, chan,
							     gircd_user);
		}

		return gircd_parse_line_privmsg_channel(dl_text, chan,
							gircd_user);
	} else {
/* msg a person */
		return gircd_parse_line_privmsg_personal(dl_text, chan,
							 gircd_user);
	}

	return 1;
}

int
gircd_parse_line_privmsg_channel(dlist_t ** dl_text, char *chan_and_text,
				 gircd_user_t * gircd_user)
{

	dlist_t *dl_text_new = NULL;
	dlist_t *dptr_chan = NULL;
	gircd_t *gircd = NULL;
	gircd_chan_t *gircd_chan = NULL;

	char *tok_chan;
	char *tok_text;

	int n;

	debug(NULL, "gircd_parse_line_privmsg_channel: Entered: [%s]\n",
	      chan_and_text);

	if (!dl_text || !sNULL(chan_and_text) || !gircd_user) {
		return -1;
	}

	gircd = gircd_user->gircd_ptr;

	tok_chan = strtok(chan_and_text, " ");
	if (!sNULL(tok_chan)) {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, "NULL");
		return -1;
	}

	dptr_chan = gircd_find_chan(gircd, tok_chan);
	if (!dptr_chan) {
		gircd_resp_chan_noexist(dl_text, gircd_user, tok_chan);
		return -1;
	}

	gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan);
	if (!gircd_chan) {
		gircd_resp_chan_noexist(dl_text, gircd_user, tok_chan);
		return -1;
	}

	tok_text = strtok(NULL, "");
	if (!sNULL(tok_text)) {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, tok_chan);
	}

	if (tok_text[0] != ':') {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, tok_chan);
	}

	tok_text = tok_text + 1;

	dl_str_unite(&dl_text_new, ":%s!~%s@%s PRIVMSG %s :%s\n",
		     gircd_user->nick, gircd_user->username,
		     gircd_user->host_name, tok_chan, tok_text);

	n = gircd_chan_tell(&dl_text_new, gircd_chan, gircd_user);

	dl_str_destroy(&dl_text_new);

	return 1;
}

int
gircd_parse_line_privmsg_personal(dlist_t ** dl_text, char *nick_and_text,
				  gircd_user_t * gircd_user)
{
	dlist_t *dl_text_new = NULL;
	dlist_t *dptr_user = NULL;
	gircd_t *gircd = NULL;
	gircd_user_t *gircd_user_ptr = NULL;

	char *tok_nick;
	char *tok_text;

	int n;

	debug(NULL, "gircd_parse_line_privmsg_personal: Entered\n");

	if (!dl_text || !sNULL(nick_and_text) || !gircd_user) {
		return -1;
	}

	gircd = gircd_user->gircd_ptr;

	tok_nick = strtok(nick_and_text, " ");
	if (!sNULL(tok_nick)) {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, "NULL");
		return -1;
	}

	dptr_user = gircd_find_user(gircd, tok_nick);
	if (!dptr_user) {
		gircd_resp_chan_noexist(dl_text, gircd_user, tok_nick);
		return -1;
	}

	gircd_user_ptr = (gircd_user_t *) dlist_data(dptr_user);
	if (!gircd_user_ptr) {
		gircd_resp_chan_noexist(dl_text, gircd_user, tok_nick);
		return -1;
	}

	tok_text = strtok(NULL, "");
	if (!sNULL(tok_text)) {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, tok_nick);
	}

	if (tok_text[0] != ':') {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, tok_nick);
	}

	tok_text = tok_text + 1;

	dl_str_unite(&dl_text_new, ":%s!~%s@%s PRIVMSG %s :%s\n",
		     gircd_user->nick, gircd_user->username,
		     gircd_user->host_name, tok_nick, tok_text);

	n = gircd_link_down_dl(gircd_user, &dl_text_new);

	dl_str_destroy(&dl_text_new);

	return 1;
}

/* FINDS */

dlist_t *gircd_find_user(gircd_t * gircd, char *nick)
{
	dlist_t *dptr_user = NULL, *dptr_tmp;
	gircd_user_t *gircd_user_ptr = NULL;

	debug(NULL, "gircd_find_user: Entered\n");

	if (!gircd || !sNULL(nick))
/* wont happen */
		return NULL;

	dlist_fornext_retarded(gircd->users, dptr_user, dptr_tmp) {
		if (!dptr_user)
			break;
		gircd_user_ptr = (gircd_user_t *) dlist_data(dptr_user);
		if (!strcasecmp(gircd_user_ptr->nick, nick)) {
			return dptr_user;
		}

	}

	return NULL;
}

dlist_t *gircd_find_chan(gircd_t * gircd, char *chan)
{
	dlist_t *dptr_chan = NULL;
	gircd_chan_t *gircd_chan_ptr = NULL;

	debug(NULL, "gircd_find_chan: Entered: %p %p, chan=%s\n", gircd, chan,
	      chan);

	if (!gircd || !sNULL(chan)) {
/* wont happen */
		printf("happening: %p %p\n", gircd, chan);
/* hah ^ */
		return NULL;
	}

	dlist_fornext(gircd->chans, dptr_chan) {
		gircd_chan_ptr = (gircd_chan_t *) dlist_data(dptr_chan);

		if (!strcasecmp(gircd_chan_ptr->name, chan)) {
			return dptr_chan;
		}

	}

	return NULL;
}

int
gircd_find_chan_user_dptr(gircd_t * gircd, gircd_chan_t * gircd_chan,
			  dlist_t * dptr_user)
{
	dlist_t *dptr, *dptr_user_ptr;

	debug(NULL, "gircd_find_chan_user_dptr: Entered\n");

	if (!gircd || !gircd_chan || !dptr_user) {
		return 0;
	}

	dlist_fornext(gircd_chan->users, dptr) {
		dptr_user_ptr = (dlist_t *) dlist_data(dptr);

		if (dptr_user_ptr == dptr_user) {
			return 1;
		}

	}

	return 0;
}

int
gircd_find_user_chan_dptr(gircd_t * gircd, gircd_user_t * gircd_user,
			  dlist_t * dptr_chan)
{
/* returns 1 if dptr_chan is found in gircd_user->chans */

	dlist_t *dptr, *dptr_chan_ptr;

	debug(NULL, "gircd_find_user_chan_dptr: Entered\n");

	if (!gircd || !gircd_user || !dptr_chan) {
		return 0;
	}

	dlist_fornext(gircd_user->chans, dptr) {
		dptr_chan_ptr = (dlist_t *) dlist_data(dptr);

		if (dptr_chan_ptr == dptr_chan) {
			return 1;
		}

	}

	return 0;
}

/* chan/user routines */

int
gircd_chan_add_user(gircd_t * gircd, gircd_chan_t * gircd_chan,
		    dlist_t * dptr_chan, gircd_user_t * gircd_user,
		    dlist_t * dptr_user)
{
	int n = 0;

	debug(NULL, "gircd_chan_add_user: Entered: %p %p %p %p %p\n", gircd,
	      gircd_chan, dptr_chan, gircd_user, dptr_user);

	if (!gircd || !gircd_chan || !gircd_user) {
		return -1;
	}

	if (dptr_chan == NULL) {
		dptr_chan = gircd_find_chan(gircd, gircd_chan->name);
	}

	if (dptr_user == NULL) {
		dptr_user = gircd_find_user(gircd, gircd_user->nick);
	}

	if (!dptr_chan || !dptr_user) {
		return -1;
	}

	if (gircd_find_chan_user_dptr(gircd, gircd_chan, dptr_user)) {
		return 0;
	}

	dlist_Dinsert_after(&gircd_chan->users, dptr_user);
	dlist_Dinsert_after(&gircd_user->chans, dptr_chan);

	return n;
}

int
gircd_chan_del_user(gircd_t * gircd, gircd_chan_t * gircd_chan,
		    dlist_t * dptr_chan, gircd_user_t * gircd_user,
		    dlist_t * dptr_user)
{
	dlist_t *dptr, *dptr_tmp, *dptr_ptr;

	debug(NULL, "gircd_chan_del_user: Entered: %p %p %p %p %p\n", gircd,
	      gircd_chan, dptr_chan, gircd_user, dptr_user);

	if (!gircd || !gircd_chan || !gircd_user) {
		return -1;
	}

	if (dptr_chan == NULL) {
		dptr_chan = gircd_find_chan(gircd, gircd_chan->name);
	}

	if (dptr_user == NULL) {
		dptr_user = gircd_find_user(gircd, gircd_user->nick);
	}

	if (!dptr_chan || !dptr_user) {
		return -1;
	}

	if (!gircd_find_chan_user_dptr(gircd, gircd_chan, dptr_user)) {
		return 0;
	}

	dlist_fornext_retarded(gircd_chan->users, dptr, dptr_tmp) {
		if (!dptr)
			break;
		dptr_ptr = (dlist_t *) dlist_data(dptr);
		if (dptr_ptr == dptr_user) {
			dlist_remove(&gircd_chan->users, dptr);
			free(dptr);
		}

	}

	dlist_fornext_retarded(gircd_user->chans, dptr, dptr_tmp) {
		if (!dptr)
			break;
		dptr_ptr = (dlist_t *) dlist_data(dptr);
		if (dptr_ptr == dptr_chan) {
			dlist_remove(&gircd_user->chans, dptr);
			free(dptr);
		}

	}

	return 1;
}

int
gircd_del_user(gircd_t * gircd, gircd_user_t * gircd_user, dlist_t * dptr_user)
{
	dlist_t *dptr, *dptr_tmp, *dptr2, *dptr2_tmp, *dptr_chan_ptr,
	    *dptr_user_ptr;
	gircd_chan_t *gircd_chan;

	debug(NULL, "gircd_del_user: Entered: %p %p %p\n", gircd, gircd_user,
	      dptr_user);

	if (!gircd || !gircd_user) {
		return -1;
	}

	if (dptr_user == NULL) {
		dptr_user = gircd_find_user(gircd, gircd_user->nick);
	}

	if (!dptr_user) {
		return -1;
	}

	gircd_user->active = 0;

	dlist_fornext_retarded(gircd_user->chans, dptr, dptr_tmp) {
		if (!dptr)
			break;
		dptr_chan_ptr = (dlist_t *) dlist_data(dptr);
		gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan_ptr);
		dlist_fornext_retarded(gircd_chan->users, dptr2, dptr2_tmp) {
			if (!dptr2)
				break;
			dptr_user_ptr = (dlist_t *) dlist_data(dptr2);
			if (dptr_user_ptr == dptr_user) {
				dlist_remove(&gircd_chan->users, dptr2);
				free(dptr2);

			}

		}
	}

	return 1;
}

char *gircd_chan_get_users(gircd_chan_t * gircd_chan)
{
	dlist_t *dptr = NULL, *dptr_user_ptr = NULL;
	gircd_user_t *gircd_user_ptr = NULL;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	if (!gircd_chan)
		return NULL;

	memset(buf, 0, sizeof(buf));
	dlist_fornext(gircd_chan->users, dptr) {
		dptr_user_ptr = (dlist_t *) dlist_data(dptr);
		gircd_user_ptr = (gircd_user_t *) dlist_data(dptr_user_ptr);
		if (!gircd_user_ptr) {
			continue;
		}
		strlcatfmt_safe(buf, sizeof(buf) - 1, "%s ",
				gircd_user_ptr->nick);
	}

	if (sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

int
gircd_chan_tell(dlist_t ** dl_text, gircd_chan_t * gircd_chan,
		gircd_user_t * gircd_user)
{
	dlist_t *dptr = NULL, *dptr_user_ptr = NULL;
	gircd_user_t *gircd_user_ptr = NULL;
	bot_t *bot_new = NULL;
	int n = 0;

	debug(NULL, "gircd_chan_tell: Entered\n");

	if (!dl_text || !gircd_chan)
		return -1;

	dlist_fornext(gircd_chan->users, dptr) {
		dptr_user_ptr = (dlist_t *) dlist_data(dptr);
		if (!dptr_user_ptr)
			continue;
		gircd_user_ptr = (gircd_user_t *) dlist_data(dptr_user_ptr);
		if (!gircd_user_ptr)
			continue;

		if (gircd_user) {
			if (gircd_user_ptr == gircd_user)	/* dont pm ourself */
				continue;
		}

		bot_new = gircd_user_ptr->bot_sub;
		n = gircd_link_down_dl(gircd_user_ptr, dl_text);
	}

	return n;
}

int gircd_chans_tell(dlist_t ** dl_text, gircd_user_t * gircd_user)
{
	dlist_t *dptr = NULL, *dptr_chan_ptr = NULL;
	gircd_chan_t *gircd_chan;

	debug(NULL, "gircd_chans_tell: Entered\n");

	if (!dl_text || !gircd_user)
		return -1;

	dlist_fornext(gircd_user->chans, dptr) {
		dptr_chan_ptr = (dlist_t *) dlist_data(dptr);
		if (!dptr_chan_ptr)
			continue;
		gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan_ptr);
		if (!gircd_chan)
			continue;

		gircd_chan_tell(dl_text, gircd_chan, gircd_user);
	}

	return 1;
}

bot_t *gircd_destroy_gmod(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gircd_t *gircd = NULL;
	gircd_user_t *gircd_user = NULL;
	dlist_t *dptr_gircd_user = NULL;

	debug(NULL, "gircd_destroy_gmod: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		debug(NULL, "GIRCD_OFF GMOD=NULL");
		return NULL;
	}

	if (!gmod->data)
		return NULL;

	switch (gmod->data_type) {
	case GIRCD_GMOD_DATA_TYPE_USER:
		{
			dptr_gircd_user = (dlist_t *) gmod->data;
			if (!dptr_gircd_user)
				break;
			gircd_user =
			    (gircd_user_t *) dlist_data(dptr_gircd_user);
			if (!gircd_user)
				break;
			bot = gircd_destroy_gmod_user(gircd_user);
			break;
		}
	case GIRCD_GMOD_DATA_TYPE_IRCD:
		{
			gircd = (gircd_t *) gmod->data;
			if (!gircd)
				break;
			bot = gircd_destroy_gmod_ircd(gircd);
			break;
		}
	default:
		break;
	}

	return bot;
}

bot_t *gircd_destroy_gmod_ircd(gircd_t * gircd)
{
	dlist_t *dptr_user = NULL, *dptr_chan, *dptr_tmp;
	gircd_user_t *gircd_user = NULL;
	gircd_chan_t *gircd_chan = NULL;

	debug(NULL, "gircd_destroy_gmod_ircd: Entered\n");

	if (!gircd)
		return NULL;

	dlist_fornext_retarded(gircd->users_unregistered, dptr_user, dptr_tmp) {
		if (!dptr_user)
			break;
		gircd_user = (gircd_user_t *) dlist_data(dptr_user);
		gircd_destroy_gmod_user(gircd_user);
	}

	dlist_fornext_retarded(gircd->users, dptr_user, dptr_tmp) {
		if (!dptr_user)
			break;
		gircd_user = (gircd_user_t *) dlist_data(dptr_user);
		gircd_destroy_gmod_user(gircd_user);
	}

	dlist_fornext_retarded(gircd->chans, dptr_chan, dptr_tmp) {
		if (!dptr_chan)
			break;
		gircd_chan = (gircd_chan_t *) dlist_data(dptr_chan);
/* FIXFIX delete chans */
	}

	if (gircd->name)
		free(gircd->name);

	if (gircd->user_ronick)
		free(gircd->user_ronick);

	gircd->gmod->data = NULL;

	memset(gircd, 0, sizeof(gircd_t));

	free(gircd);

	return NULL;
}

bot_t *gircd_destroy_gmod_user(gircd_user_t * gircd_user)
{
	dlist_t *dl_text_new = NULL;

	debug(NULL, "gircd_destroy_gmod_user: Entered\n");

	if (!gircd_user) {
		return NULL;
	}

	if (dlist_size(gircd_user->chans) > 0) {
		gircd_parse_line_quit(&dl_text_new, "Lost connection.",
				      gircd_user);
	}

	if (gircd_user->host_name)
		free(gircd_user->host_name);

	if (gircd_user->nick)
		free(gircd_user->nick);

	if (gircd_user->username)
		free(gircd_user->username);

	if (gircd_user->hostname)
		free(gircd_user->hostname);

	if (gircd_user->servername)
		free(gircd_user->servername);

	if (gircd_user->realname)
		free(gircd_user->realname);

	if (gircd_user->status == GIRCD_USER_STATUS_UNREGISTERED) {
		dlist_remove(&gircd_user->gircd_ptr->users_unregistered,
			     gircd_user->dptr_gircd_user);
	} else {
		dlist_remove(&gircd_user->gircd_ptr->users,
			     gircd_user->dptr_gircd_user);
	}

	if (gircd_user->dptr_gircd_user) {
		memset(gircd_user->dptr_gircd_user, 0, sizeof(dlist_t));
		free(gircd_user->dptr_gircd_user);
	}

	if (gircd_user->gmod) {
		gircd_user->gmod->data = NULL;
	}

	memset(gircd_user, 0, sizeof(gircd_user_t));

	free(gircd_user);

	return NULL;
}

bot_t *gircd_destroy_down(dlist_t * dlist_node, bot_t * bot)
{
	bot_t *ret = NULL;

	debug(NULL, "gircd_destroy_down: Entered\n");

	ret = gircd_destroy_gmod(dlist_node, bot);

	gmodule_destroy_down(dlist_node, bot);

	return ret;
}

bot_t *gircd_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_t *ret = NULL;

	debug(NULL, "gircd_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	ret = gircd_destroy_gmod(dlist_node, bot);

	return ret;
}

bot_t *gircd_control_down(dlist_t * dlist_node, bot_t * bot)
{
	bot_t *ret = NULL;

	debug(NULL, "gircd_control_down: Entered\n");

	gmodule_control_down(dlist_node, bot);

	return ret;
}

bot_t *gircd_control_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_t *ret = NULL;

	debug(NULL, "gircd_control_up: Entered\n");

	gmodule_control_up(dlist_node, bot);

	return ret;
}

/* 
 * BOT FUNCS 
 */

/*
 * SPECIAL CHANS *
 */
int
gircd_special_chan_pm_console(dlist_t ** dl_text, char *chan_and_text,
			      gircd_user_t * gircd_user)
{
	dlist_t *dl = NULL, *dptr = NULL, *dptr_tok = NULL;
	gircd_user_t *gircd_user_robot = NULL;
	char *str = NULL, *str_new = NULL, *tok_chan = NULL, *tok_text = NULL;
	int n = 0;

	debug(NULL, "gircd_special_chan_pm_console: Entered\n");

	if (!dl_text || !sNULL(chan_and_text) || !gircd_user)
		return -1;

	if (!sNULL(gircd_user->gircd_ptr->user_robot))
		return -1;

	gircd_user_robot = (gircd_user_t *) gircd_user->gircd_ptr->user_robot;

	tok_chan = strtok(chan_and_text, " ");
	if (!sNULL(tok_chan)) {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, "NULL");
		return -1;
	}

	tok_text = strtok(NULL, "");
	if (!sNULL(tok_text)) {
/* illegal */
		gircd_resp_chan_illegal(dl_text, gircd_user, "NULL");
		return -1;
	}

	tok_text = tok_text + 1;

	bot_dl_print_flush(gircd_user->bot_sub);
	console_parse(gircd_user->bot_sub, tok_text);
	bot_dl_print_change_strings(gircd_user->bot_sub, "\t", " ");

	dlist_fornext(gi->dl_print, dptr) {

		if (!dptr->data)
			continue;

		str = (char *)dlist_data(dptr);

		dl = tokenize(NULL, str,
			      TOKENIZE_NORMAL | TOKENIZE_LEAVESEP |
			      TOKENIZE_MATCHANY, "\r\n");
		if (!dl)
			continue;

		dlist_fornext(dl, dptr_tok) {
			str = (char *)dlist_data(dptr_tok);
			str_new = str_unite("%s :%s", tok_chan, str);

			n = gircd_parse_line_privmsg_channel(dl_text, str_new,
							     gircd_user_robot);
			free(str_new);
		}
		tokenize_destroy(NULL, &dl);
	}

	bot_dl_print_destroy(NULL);

	if (dl)
		tokenize_destroy(NULL, &dl);

	return n;
}

gircd_user_t *gircd_special_user_init(gircd_t * gircd, char *name)
{
	dlist_t *dptr_gmod = NULL;
	dlist_t *dptr_user = NULL;
	gircd_user_t *gircd_user = NULL;
	bot_gmod_elm_t *gmod_sub = NULL;
	debug(NULL, "gircd_special_user_init: Entered\n");
	if (!gircd || !sNULL(name))
		return NULL;

	gircd_user = (gircd_user_t *) calloc(1, sizeof(gircd_user_t));
	if (!gircd_user)
		return NULL;
	gircd_user->nick = strdup(name);
	gircd_user->active = 1;
	gircd_user->gmod = gmod_sub;
	gircd_user->dptr_gmod = dptr_gmod;
	gircd_user->gircd_ptr = gircd;
	gircd_user->status = GIRCD_USER_STATUS_REGISTERED;
	time(&gircd_user->time_logon);
	gircd_user->host_name = strdup(gircd->name);

	dptr_user = dlist_Dinsert_after(&gircd->users, gircd_user);

/* bot_sub's gmod gets a dlist_t, not the gircd_user, this way, can delete from unregistered very fast, and insert into registered users very fast */

	gircd_user->dptr_gircd_user = dptr_user;

	gircd->user_robot = gircd_user;

	return gircd_user;
}
