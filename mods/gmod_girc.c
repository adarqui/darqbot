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
#include "gmod_girc.h"

/* FIX FOR INCLUDING GIRC DATA FROM OTHER HEADERS */

module_t gmod_girc_info;

void __girc_init__(void) __attribute__ ((constructor));

void __girc_init__(void)
{

	strlcpy_buf(gmod_girc_info.name, "gmod_girc");
	strlcpy_buf(gmod_girc_info.trigger, "^girc");
	gmod_girc_info.init = girc_init;
	gmod_girc_info.fini = girc_fini;
	gmod_girc_info.help = girc_help;
	gmod_girc_info.run = girc_run;
	gmod_girc_info.control = NULL;

	gmod_girc_info.output = girc_output;
	gmod_girc_info.input = girc_input;

	gmod_girc_info.off = girc_off;

	gmod_girc_info.destroy_down = girc_destroy_down;
	gmod_girc_info.destroy_up = girc_destroy_up;
	gmod_girc_info.control_up = girc_control_up;
	gmod_girc_info.control_down = girc_control_down;

	gmod_girc_info.self = &gmod_girc_info;

	debug(NULL, "__irc_init__: Loaded gmod_irc\n");

	return;
}

bot_t *girc_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "irc_init: Entered\n");
	return NULL;
}

bot_t *girc_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "irc_fini: Entered\n");
	return NULL;
}

bot_t *girc_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "girc_off: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		debug(NULL, "girc_off: GIRC OFF GMOD = NULL\n");
		return NULL;
	}

	girc_destroy_gmod(gmod);

	return NULL;
}

bot_t *girc_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "irc_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^irc";

	return NULL;
}

bot_t *girc_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	girc_t *girc = NULL;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "irc_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "irc_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_girc_info.trigger,
					       gmod_girc_info.trigger_ext);
	if (!dptr_gmod) {
		debug(NULL, "girc_run: GIRC DPTR=NULL\n");
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "girc_run: GIRC GMOD = NULL\n");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL, "girc_run: GIRC GMOD->DATA = NULL, ALLOCATING\n");
		girc_gmod_init(bot, gmod, dptr_gmod);
		girc = gmod->data;
	} else
		girc = gmod->data;

	MOD_OPTIONS_TOP_HALF;

	girc_process_options(girc, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

	girc_defaults_post(girc);

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	girc_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *girc_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char *sep_ptr;

	debug(bot, "irc_change_string: Entered\n");

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	return str;
}

char *girc_process_options(girc_t * girc, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "girc_process_options: Entered\n");

	if (!girc || !_sNULL(string))
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	dl = tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		      "...");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		girc_process_options_parse(girc, (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void girc_process_options_parse(girc_t * girc, char *string)
{

	if (!girc || !_sNULL(string))
		return;

	debug(NULL, "girc_process_options_parse: Entered\n");

	if (!strncasecmp_len(string, "nick=")) {
		girc_process_options_parse_nick(girc, &string[strlen("host=")]);
	} else if (!strncasecmp_len(string, "channel=")) {
		girc_process_options_parse_channel(girc,
						   &string[strlen("channel=")]);
	}

	gmodule_parse_common_options(girc->bot_root, girc->gmod, string);

	return;
}

void girc_process_options_parse_nick(girc_t * girc, char *string)
{
	nick_t *nick;
	char *tok_1, *tok_2, *tok_3, *tok_4, *tok_5;

	if (!girc || !_sNULL(string))
		return;

	tok_1 = strtok(string, ":");
	if (!tok_1)
		return;

	tok_2 = strtok(NULL, ":");
	if (!tok_2)
		return;

	tok_3 = strtok(NULL, ":");
	if (!tok_3)
		return;

	tok_4 = strtok(NULL, ":");
	if (!tok_4)
		return;

	tok_5 = strtok(NULL, "");
	if (!tok_5)
		return;

	nick = (nick_t *) calloc(1, sizeof(nick_t));
	nick->nick = strdup(tok_1);
	nick->username = strdup(tok_2);
	nick->hostname = strdup(tok_3);
	nick->servername = strdup(tok_4);
	nick->realname = strdup(tok_5);

	dlist_Dinsert_after(&girc->dl_nick, nick);

	return;
}

void girc_process_options_parse_channel(girc_t * girc, char *string)
{
	channel_t *channel;
	dlist_t *dl, *dptr;
	char *tok_1, *tok_2;

	if (!girc || !_sNULL(string))
		return;

	dl = tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		      ",");
	if (!dl)
		return;

	dlist_fornext(dl, dptr) {
		tok_1 = (char *)dlist_data(dptr);

		if (!tok_1)
			continue;

		tok_2 = _strchr(tok_1, ':');

		channel = (channel_t *) calloc(1, sizeof(channel_t));

		channel->name =
		    strdup_len(tok_1,
			       tok_2 == NULL ? strlen(tok_1) : (tok_2 - tok_1));
		if (tok_2)
			channel->key = strdup(tok_2 + 1);

		dlist_Dinsert_after(&girc->dl_channel, channel);
	}

	tokenize_destroy(NULL, &dl);

	return;
}

bot_t *girc_output(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "irc_output: Entered\n");

	girc_send_dptr(dlist_node);

	return NULL;
}

void *girc_input_iohooks(void *pa, void *pb)
{
	dlist_t *dptr_mod = NULL;
	module_t *mod = NULL;
	bot_t *bot = NULL;

	debug(NULL, "girc_input_iohooks: Entered\n");

	if (!pa || !pb)
		return NULL;

	bot = (bot_t *) pa;
	if (!bot)
		return NULL;

	dptr_mod = (dlist_t *) pb;
	mod = (module_t *) dlist_data(dptr_mod);
	if (!mod)
		return NULL;

	if (mod->input)
		mod->input(dptr_mod, bot);

	return NULL;
}

bot_t *girc_input(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dl, *dptr_tok;
	girc_t *girc = NULL;
	bot_gmod_elm_t *gmod;

	int n;
	char buf[MAX_BUF_SZ + 1], *buf_ptr;

	debug(bot, "irc_input: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	if (!gmod->data) {
		girc_run(dlist_node, bot);
	}

	girc = gmod->data;
	if (!girc) {
		debug(NULL, "girc_input: GIRC_INPUT GIRC=NULL");
		return NULL;
	}

	_memset(buf, 0, sizeof(buf));
	memcopy(buf, bot->txt_data_in, sizeof(buf) - 1, bot->txt_data_in_sz);

	n = bot->txt_data_in_sz;
	buf[n] = '\0';

	dl = tokenize(bot, buf, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES, "\n");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr_tok) {
		buf_ptr = (char *)dlist_data(dptr_tok);

		debug(bot, "girc_evhook_link: line [\n%s]\n", buf_ptr);

		if (!strncasecmp_len
		    (buf_ptr, "NOTICE AUTH :*** Processing connection to")) {
			girc_send_nickinfo(girc);
		}
		if (girc_line_info(girc, buf_ptr, strlen(buf_ptr)) < 0) {
			debug(bot, "girc_evhook_link: bot_line_info: Failed\n");
			continue;
		}

/* hook raw input module processing */
		_strstrip_nl(bot->txt_data_in);
		dlist_traverse(&gi->dl_mod_iohooks, girc_input_iohooks, bot);

		if (bot->isprivmsg) {
			debug(bot,
			      "girc_evhook_link: privmsg caught: nick=%s, ident=%s, host=%s, to=%s, message=%s\n",
			      bot->txt_nick, bot->txt_ident, bot->txt_host,
			      bot->txt_to, bot->txt_data_in);
		}

		if (strstr(buf_ptr, "NOTICE AUTH :*** Checking Ident")
		    && !bot->isprivmsg) {
			girc_send_nickinfo(girc);
			girc_send_autojoins(girc);
			continue;
		}

		else if (!strncasecmp_len(buf_ptr, "ping ") && !bot->isprivmsg) {
			char *str;
			char *pong_reply = &buf_ptr[_strlen("ping :")];

			str = str_unite_static("PONG %s\n", pong_reply);

			memcopy(bot->txt_data_out, str,
				sizeof(bot->txt_data_out) - 1, strlen(str) + 1);
			bot->txt_data_out_sz = strlen(bot->txt_data_out);
			gmodule_down(dlist_node, bot);

			debug(bot, "girc_evhook_link: MATCHED PING: sending %s",
			      str);

			girc_send_autojoins(girc);

			continue;
		} else if (!strncasecmp_len(buf_ptr, "pong ")
			   && !bot->isprivmsg) {
/* SPECIAL CASE: this is for replying to already-replied-to relinkc/ircpong's */

			girc_send_autojoins(girc);

			continue;
		}

		if (bot->mute) {
			bot_line_clear(bot);
			return NULL;
		}

		if (bot->isline) {

			_strstrip_nl(bot->txt_data_in);

/* hooks: input module processing */

			bot->var_nesting_cur = 0;

			gmodule_up(dlist_node, bot);

			if (_sNULL(bot->txt_data_out) != NULL && bot->shouldsend
			    && _sNULL(bot->txt_to) != NULL) {

				girc_send(girc);

			}

			if (bot->isforked)
				bot_fork_clean_exit(bot);
		}
	}

	bot_line_clear(bot);

	if (dl)
		tokenize_destroy(bot, &dl);

	return bot;
}

int girc_send_nickinfo(girc_t * girc)
{
	bot_t *bot = NULL;
	nick_t *nick;
	char *str;

	debug(bot, "girc_send_nickinfo: Entered\n");

	if (!girc)
		return -1;

	bot = girc->bot_root;

	girc_change_nick(girc);

	nick = (nick_t *) dlist_data(girc->dl_nick_cur);

	str = str_unite_static("NICK %s\n", nick->nick);

	debug(NULL, "girc_send_nickinfo: sending %s", str);

	memcopy(bot->txt_data_out, str, sizeof(bot->txt_data_out) - 1,
		strlen(str) + 1);
	bot->txt_data_out_sz = strlen(bot->txt_data_out);
	gmodule_down(girc->dptr_gmod, bot);

	str =
	    str_unite_static("USER %s %s %s %s\n", nick->username,
			     nick->hostname, nick->servername, nick->realname);

	debug(NULL, "girc_send_nickinfo: sending %s", str);

	memcopy(bot->txt_data_out, str, sizeof(bot->txt_data_out) - 1,
		strlen(str) + 1);
	bot->txt_data_out_sz = strlen(bot->txt_data_out);
	gmodule_down(girc->dptr_gmod, bot);

	return 0;
}

int girc_send_autojoins(girc_t * girc)
{
	bot_t *bot = NULL;
	channel_t *channel;
	dlist_t *dptr;
	char *str;

	debug(bot, "girc_send_autojoins: Entered\n");
	if (!girc)
		return -1;

	bot = girc->bot_root;

	dlist_fornext(girc->dl_channel, dptr) {
		channel = (channel_t *) dlist_data(dptr);
		if (channel->key)
			str =
			    str_unite_static("JOIN %s %s\n", channel->name,
					     channel->key);
		else
			str = str_unite_static("JOIN %s\n", channel->name);

		memcopy(bot->txt_data_out, str, sizeof(bot->txt_data_out) - 1,
			strlen(str) + 1);
		bot->txt_data_out_sz = strlen(bot->txt_data_out);
		gmodule_down(girc->dptr_gmod, bot);

		usleep(20000);
	}

	return 0;
}

int girc_change_nick(girc_t * girc)
{

	if (!girc)
		return -1;

	if (!girc->dl_nick_cur) {
		girc->dl_nick_cur = girc->dl_nick;
		return 0;
	}

	girc->dl_nick_cur =
	    (girc->dl_nick_cur->next ==
	     NULL) ? girc->dl_nick : girc->dl_nick_cur->next;

	return 0;
}

int girc_line_info(girc_t * girc, char *line, int len)
{
	bot_t *bot = NULL;
	char *tok_1, *tok_2, *tok_3, *tok_4, *tok_5, *tok_6, *tok_rest;
	char *line_dup, *line_dup_ptr;

	debug(bot, "girc_line_info: Entered\n");

	tok_1 = tok_2 = tok_3 = tok_4 = tok_5 = tok_6 = tok_rest = NULL;

	line_dup = strdup(line);

	if (!girc || !_sNULL(line) || len <= 0)
		return -1;

	bot = girc->bot_root;

	line_dup_ptr = line_dup;

/* nick or server */
	tok_1 = strtok(line_dup_ptr, " ");
	if (!_sNULL(tok_1))
		goto cleanup;

	tok_rest = tok_1 + _strlen(tok_1) + 1;

	if (strstr(tok_1, "!")) {
/* line */

		tok_1 = strtok(tok_1, "!");

		if (tok_1[0] == ':')
			tok_1 = tok_1 + 1;

		debug(bot, "1=%s\n", tok_1);

/* ident */
		tok_2 = strtok(NULL, "@");
		if (!_sNULL(tok_2))
			goto cleanup;

		debug(bot, "2=%s\n", tok_2);

/* host */
		tok_3 = strtok(NULL, " ");
		if (!_sNULL(tok_3))
			goto cleanup;

		debug(bot, "3=%s\n", tok_3);

/* command */
		tok_4 = strtok(tok_rest, " ");
		if (!_sNULL(tok_4))
			goto cleanup;

		debug(bot, "4=%s\n", tok_4);

		if (!strncasecmp_len(tok_4, "PRIVMSG")) {
			strlcpy_buf(bot->irc_command, "PRIVMSG");

			bot->isline = BOT_LINE_PRIVMSG;
			bot->isprivmsg = 1;
		} else if (!strncasecmp_len(tok_4, "KICK")) {
			strlcpy_buf(bot->irc_command, "KICK");
			bot->isline = BOT_LINE_KICK;
		} else if (!strncasecmp_len(tok_4, "BAN")) {
			strlcpy_buf(bot->irc_command, "BAN");
			bot->isline = BOT_LINE_BAN;
		} else if (!strncasecmp_len(tok_4, "MODE")) {
			strlcpy_buf(bot->irc_command, "MODE");
			bot->isline = BOT_LINE_MODE;
		} else if (!strncasecmp_len(tok_4, "TOPIC")) {
			strlcpy_buf(bot->irc_command, "TOPIC");
			bot->isline = BOT_LINE_TOPIC;
		} else if (!strncasecmp_len(tok_4, "JOIN")) {
			strlcpy_buf(bot->irc_command, "JOIN");
			bot->isline = BOT_LINE_JOIN;
		} else if (!strncasecmp_len(tok_4, "PART")) {
			strlcpy_buf(bot->irc_command, "PART");
			bot->isline = BOT_LINE_PART;
		} else if (!strncasecmp_len(tok_4, "QUIT")) {
			strlcpy_buf(bot->irc_command, "QUIT");
			bot->isline = BOT_LINE_QUIT;
		} else if (!strncasecmp_len(tok_4, "NICK")) {
			strlcpy_buf(bot->irc_command, "NICK");
			bot->isline = BOT_LINE_NICK;
		}

/* to */
		tok_5 = strtok(NULL, " ");
		if (!_sNULL(tok_5)) {
			goto cleanup;
		}

		if (bot->isline == BOT_LINE_NICK)
			goto copy;

		if (tok_5[0] != '#') {
/* private message */
			if (!bot->var_allowpm)
				goto cleanup;

			tok_5 = tok_1;

		}

		debug(bot, "5=%s\n", tok_5);

/* message */
		tok_6 = strtok(NULL, "");
		if (tok_6) {

			if (bot->isprivmsg)
				tok_6 = tok_6 + 1;

			debug(bot, "6=%s\n", tok_6);
		}

 copy:

		strlcpy_buf(bot->txt_nick, tok_1);
		strlcpy_buf(bot->txt_ident, tok_2);
		strlcpy_buf(bot->txt_host, tok_3);
		strlcpy_buf(bot->txt_to, tok_5);

		if (tok_6) {
			strlcpy_buf(bot->txt_data_in, tok_6);
		}

	} else {

		if (*tok_1 == ':')
			tok_1++;

		tok_2 = strtok(NULL, " ");
		if (!_sNULL(tok_2))
			goto cleanup;

		tok_3 = strtok(NULL, " ");
		if (!_sNULL(tok_3))
			goto cleanup;

		tok_4 = strtok(NULL, "");
		if (!_sNULL(tok_4))
			goto cleanup;

		strlcpy_buf(bot->txt_server, tok_1);
		bot->txt_server_msg = atoi(tok_2);
		strlcpy_buf(bot->txt_nick, tok_3);
		strlcpy_buf(bot->txt_data_in, tok_4);

		debug(bot, "1=%s\n2=%s\n3=%s\n4=%s\n", tok_1, tok_2, tok_3,
		      tok_4);
	}

	free(line_dup);
	line_dup = NULL;
	return 0;

 cleanup:
	bot_line_clear(bot);
	if (line_dup)
		free(line_dup);

	return 0;
}

int girc_send_dptr(dlist_t * dptr_girc)
{
	bot_gmod_elm_t *gmod = NULL;
	girc_t *girc = NULL;
	int n;

	debug(NULL, "girc_send_dptr: Entered\n");

	if (!dptr_girc) {
		return -1;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_girc);
	if (!gmod)
		return -1;

	girc = (girc_t *) gmod->data;

	n = girc_send(girc);

	return n;
}

int girc_send(girc_t * girc)
{
	bot_t *bot = NULL;
	dlist_t *dlist_node = NULL;
	dlist_t *dl, *dptr;
	char *str_ptr = NULL, *str_ptr_new = NULL;
	int n, o;

	debug(bot, "girc_send: Entered\n");

	if (!girc)
		return -1;

	bot = girc->bot_root;
	dlist_node = girc->dptr_gmod;

	n = 0;

	debug(NULL, "girc_send: RAW DATA=[%s]\n", bot->txt_data_out);

	if (!bot->var_multi) {
		str_clean_nl(bot->txt_data_out, _strlen(bot->txt_data_out));

		if (_strlen(bot->txt_data_out) > bot->var_bufsz) {
			bot->txt_data_out[bot->var_bufsz - 1] = '\n';
			bot->txt_data_out[bot->var_bufsz] = '\0';
		}

		if (bot->isprivmsg) {
			str_ptr =
			    str_unite_static("PRIVMSG %s :%s\n", bot->txt_to,
					     bot->txt_data_out);
		} else {
			str_ptr = str_unite_static("%s\n", bot->txt_data_out);
		}

		memcopy(bot->txt_data_out, str_ptr,
			sizeof(bot->txt_data_out) - 1, strlen(str_ptr) + 1);
		bot->txt_data_out_sz = strlen(bot->txt_data_out);
		gmodule_down(dlist_node, bot);

		debug(NULL, "girc_send: (graw) SENDING [%s]\n", str_ptr);

		if (n <= 0)
			return n;

	}

	else {
/* multi line */
		dl = tokenize(bot, bot->txt_data_out,
			      TOKENIZE_NORMAL | TOKENIZE_LEAVESEP |
			      TOKENIZE_LEAVEQUOTES, "\n");
		if (!dl)
			return 0;

		o = 0;
		dlist_fornext(dl, dptr) {
			str_ptr = (char *)dlist_data(dptr);

			if (o >= bot->var_maxflood) {
				break;
			}

			n = 1;
			str_ptr_new = NULL;

			if (_strlen(str_ptr) > bot->var_bufsz) {
				str_ptr[bot->var_bufsz] = '\0';
			}

			if (_strlen(str_ptr) > 1) {
				if (bot->isprivmsg) {
					str_ptr_new =
					    str_unite_static("PRIVMSG %s :%s\n",
							     bot->txt_to,
							     str_ptr);
				} else {
					str_ptr_new =
					    str_unite_static("%s\n", str_ptr);
				}
				memcopy(bot->txt_data_out, str_ptr_new,
					sizeof(bot->txt_data_out),
					strlen(str_ptr_new) + 1);
				bot->txt_data_out_sz =
				    strlen(bot->txt_data_out);
				gmodule_down(dlist_node, bot);

				debug(NULL, "girc_send: (girc2) SENDING [%s]\n",
				      str_ptr_new);
			}

			o++;
			usleep(bot->var_usleep);

			if (n <= 0)
				return n;

		}

		tokenize_destroy(bot, &dl);
	}

	return n;
}

void girc_defaults_post(girc_t * girc)
{
	if (!girc)
		return;

	debug(NULL, "girc_defaults: Entered\n");

/* put these in .darqbot/conf, setup parser for them */
/* XXX
  bot->var_usleep = 80000;
  bot->var_maxbufsz = 512;
  bot->var_bufsz = bot->var_maxbufsz;
  bot->var_maxflood = 5;
  bot->var_maxnesting = 5;
  bot->var_nesting = 3;
*/

	return;
}

void girc_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	girc_t *girc = NULL;

	debug(NULL, "girc_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	girc = (girc_t *) calloc(1, sizeof(girc_t));

	if (!girc)
		return;

	girc->gmod = gmod;
	girc->dptr_gmod = dptr_gmod;

	gmod->data = girc;

	girc->bot_root = bot;

	return;
}

bot_t *girc_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	girc_t *girc = NULL;

	debug(NULL, "girc_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			girc = gmod->data;
			girc_destroy_gmod(gmod);
		}
	}

	return bot;
}

bot_t *girc_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	girc_t *girc = NULL;

	debug(NULL, "girc_destroy_down: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			girc = gmod->data;
			girc_destroy_gmod(gmod);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void girc_destroy_gmod(bot_gmod_elm_t * gmod)
{
	girc_t *girc = NULL;

	debug(NULL, "girc_destroy_gmod: Entered\n");
	if (!gmod)
		return;

	girc = (girc_t *) gmod->data;
	if (!girc)
		return;

	dlist_fini(&girc->dl_nick, nick_free);
	dlist_fini(&girc->dl_channel, channel_free);

	free(girc);

	gmod->data = NULL;

	return;
}

bot_t *girc_control_up(dlist_t * dlist_node, bot_t * bot)
{

	debug(NULL, "girc_control_up: Entered\n");

	gmodule_control_up(dlist_node, bot);

	return bot;
}

bot_t *girc_control_down(dlist_t * dlist_node, bot_t * bot)
{

	debug(NULL, "girc_control_down: Entered\n");

	gmodule_control_down(dlist_node, bot);

	return bot;
}
