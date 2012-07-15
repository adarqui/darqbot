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
#include "bot.h"

bot_t *bot_init(void)
{
	dlist_t *dptr;
	bot_t *bot;

	debug(NULL, "bot_init: Entered\n");

	bot = (bot_t *) calloc(1, sizeof(bot_t));
	if (!bot)
		return (NULL);

	bot_defaults(bot);

	dptr = dlist_Dinsert_after(&global_info.bots, bot);

	bot->dptr_self = dptr;

	bot->node_type = BOT_NODE_TYPE_ROOT;

	bot->ID = rand();

	return (bot);
}

void bot_fini_subs(void *bot_arg)
{
	bot_t *bot = (bot_t *) bot_arg;

	if (!bot)
		return;

	debug(NULL, "bot_fini_subs: Entered: tag=%s\n", bot->tag);

	if (bot->node_type != BOT_NODE_TYPE_ROOT)
		return;

/*
	dlist_fornext_retarded(bot->dl_subs, dptr_sub, dptr_sub_tmp) {
		if (!dptr_sub)
			break;
		bot = (bot_t *) dlist_data(dptr_sub);
		bot_fini(bot);
	}
*/
dlist_fini(&bot->dl_subs, bot_fini);

	return;
}

void bot_fini(void *bot_arg)
{
	bot_t *bot = (bot_t *) bot_arg;

	if (!bot)
		return;

	debug(bot, "bot_fini: Entered: tag=%s\n", bot->tag);

	if (bot->isforked)
		bot_fork_clean_exit(bot);

	if (bot->node_type == BOT_NODE_TYPE_ROOT) {
		debug(bot, "bot_fini: ROOT NODE\n");

		dlist_fini(&bot->dl_subs, bot_fini);

		tokenize_destroy(bot, &bot->dl_pmodules);

		environ_destroy(bot);

		if (bot->dptr_self) {
			dlist_remove(&gi->bots, bot->dptr_self);
			free(bot->dptr_self);
			bot->dptr_self = NULL;
		}

		bz(bot->trig_prefix);

		gmodule_off(bot);

		_memset(bot, 0, sizeof(bot_t));

		free(bot);
	} else if (bot->node_type == BOT_NODE_TYPE_SUB) {
		debug(bot, "bot_fini: NODE SUB\n");

		gmodule_off(bot);

/* free the damn bots */

		dlist_fini(&bot->dl_subs, bot_fini);

	}

	return;
}

void bot_exit_handler(int sig)
{
	debug(NULL, "bot_exit_handler: Entered\n");

	exit(0);
}

void bot_atexit_handler(void)
{

	debug_print_backtrace();

	debug(NULL, "bot_atexit_handler: Entered\n");

	xpid_waitpid();

	ERR_free_strings();

	global_off();

	exit(gi->atexit_code);

	return;
}

void bot_sigend_handler(int num)
{
	char *sig_str = NULL;

	signal(num, bot_sigend_handler);

	sig_str = sig_itos(num);
	debug(NULL,
	      "bot_sigend_handler: caught signum=%i (%s) ::: sleeping for 15 seconds\n",
	      num, sig_str);

	debug_print_backtrace();

	if (gi->var_sigprotect) {
/*
      longjmp (gi->sigprotect_buf, 1);
*/
		siglongjmp(gi->sigprotect_sigbuf, 1);
	}

/*

  sleep (15);

  exit (0);
*/
	return;
}

void bot_sigpipe_handler(int num)
{

	xpid_waitpid();

	debug(NULL, "bot_sigpipe_handler: Entered: pid=%i\n", getpid());

/* reset signal just in case */
	signal(SIGPIPE, bot_sigpipe_handler);

	return;
}

void bot_sigchld_handler(int num)
{

	xpid_waitpid();

	debug(NULL, "bot_sigchld_handler: Entered: pid=%i\n", getpid());

/* reset signal just in place */
	signal(SIGCHLD, bot_sigchld_handler);

	return;
}

void bot_sigalrm_handler(int num)
{

	xpid_waitpid();

	debug(NULL, "bot_sigalrm_handler: Entered: pid=%i\n", getpid());

/* reset signal just in place */
	signal(SIGALRM, bot_sigalrm_handler);

	return;
}

void bot_fini_wrapper(void)
{
	dlist_t *dptr, *dptr_tmp;
	bot_t *bot;

	debug(NULL, "bot_fini_wrapper: Entered\n");

	dlist_fornext_retarded(global_info.bots, dptr, dptr_tmp) {
		if (!dptr)
			break;

		bot = dlist_data(dptr);
		if (bot)
			bot_fini(bot);
	}

	return;
}

int bot_line_clear(bot_t * bot)
{
	if (!bot)
		return (-1);

	debug(bot, "bot_line_clear: Entered\n");

/* FIX, too much _memsetting */
	bz(bot->irc_command);
	bz(bot->txt_nick);
	bz(bot->txt_ident);
	bz(bot->txt_host);
	bz(bot->txt_to);
	bz(bot->txt_data_in);
	bz(bot->txt_data_out);
	bz(bot->trig_called);

	bot->txt_data_in_sz = 0;
	bot->txt_data_out_sz = 0;

	bot->isprivmsg = 0;
	bot->shouldsend = 0;
	bot->isline = 0;
	bot->isforked = 0;
	bot->isforked_stop = 0;
	bot->var_multi = 0;
	bot->ispm = 0;
	bot->iscomment = 0;

	bot->dl_gmodules_force = 0;

	environ_destroy_args(bot);

	return (0);
}

int bot_line_clear_bufs(bot_t * bot)
{
	if (!bot)
		return (-1);

	debug(bot, "bot_line_clear_bufs: Entered\n");

/* FIX, too much _memsetting */
	bz(bot->txt_data_in);
	bz(bot->txt_data_out);

	bot->txt_data_in_sz = 0;
	bot->txt_data_out_sz = 0;

	return (0);
}

int bot_shouldreturn(bot_t * bot)
{

	if (!bot)
		return (1);

	if (bot->iscomment)
		return (1);

	return (0);
}

void bot_find_and_set_fd(char *tag, int fd)
{

	dlist_t *dptr_gmod = NULL;
	bot_t *bot, *bot_tag;

	dlist_t *dptr_control = NULL;
	control_t *control = NULL;

	char *tok_tag = NULL, *tok_id = NULL, *tok_tag_ext = NULL;
	int tok_id_val = 0;

	bot_t *bot_tmp;

	bot_tmp = dlist_data(gi->bots);
	console_print_all(0);
	bot_dl_print_flush(bot_tmp);

	debug(NULL, "bot_find_and_set_fd: Entred: tag=%s\n", tag);

	if (!_sNULL(tag) || fd < 0) {
		return;
	}

	tok_tag = strtok(tag, ",");
	if (!_sNULL(tok_tag))
		return;

	tok_id = strtok(NULL, ",");
	if (!_sNULL(tok_id))
		return;

	tok_id_val = atoi(tok_id);

	tok_tag_ext = strtok(NULL, "");

	bot_tag = bot_find_tag(tok_tag);
	if (!bot_tag)
		return;

	bot = bot_find_sub_by_id(bot_tag, tok_id_val);

	if (bot) {
		debug(bot, "bot_find_and_set_fd: MATCH: tag=%s, fd=%i\n", tag,
		      fd);

		control = control_init();
		control_add_fdpass(control, fd);
		dptr_control = control_bot_add(bot, control);

		if (_sNULL(tok_tag_ext)) {
			dptr_gmod =
			    gmodule_find_gmod_dptr(bot, NULL, tok_tag_ext);
			if (dptr_gmod) {
				if (dlist_prev(dptr_gmod))
					dptr_gmod = dlist_prev(dptr_gmod);
				else
					dptr_gmod = NULL;
			}
		}

		gmodule_control_up(dptr_gmod, bot);

		control_bot_del(bot, dptr_control);
	}

	return;
}

bot_t *bot_init_and_turn_on(char *tag)
{
	bot_t *bot = NULL;
	char filepath[132];
	struct stat st;

	if (strstr(tag, ".conf") == NULL) {
		tag = str_unite_static("%s.conf");
	}

	if (!_sNULL(tag))
		return NULL;

	bz(filepath);

	snprintf_buf(filepath, "%s%s", gi->confdir, tag);

	bz2(st);

	if (stat(filepath, &st) != 0)
		return NULL;

	bot = bot_init();
	if (!bot) {
		debug(NULL, "main: bot_init\n");
	}

	strlcpy_buf(bot->confdir, "/.darqbot/");
	strlcpy_buf(bot->conffile, filepath);

	bot_turn_on(bot, -1);
	return NULL;
}

void bot_turn_on(bot_t * bot, int on_or_off)
{
	bot_t *bot_sub = NULL;

	debug(bot, "bot_turn_on: Entered\n");

	if (!bot)
		return;

	debug(NULL, "bot_turn_on: bot_find_confdir=success, confdir=%s\n",
	      bot->confdir);

	if (on_or_off > 0) {
		bot_turn_off(bot);
	}

	if (conf_parse(bot) < 0) {
		debug(NULL, "bot_turn_on: conf_parse: Failed\n");
	}

	if (on_or_off > 0)
		bot->on = 1;
	else if (on_or_off == 0)
		bot->on = 0;
	else {
/* untouched, -1
*/
	}

	bot_dl_print_flush(bot);
	console_info_print(bot);
	bot_dl_print_flush(bot);

	if (on_or_off > 0) {
		bot_sub = bot_dup_sub(bot);
		bot_sub_add(bot, bot_sub);

		if (bot_sub->dl_gmodules) {
			bot_gmodule_run_stack_new_once(bot_sub);
		} else {
			debug(NULL, "UNSUPPORTED");
		}
	}

	return;
}

void bot_turn_off(bot_t * bot)
{

	debug(NULL, "bot_turn_off: Entered\n");

	if (!bot)
		return;

	if (bot->isforked)
		bot_fork_clean_exit(bot);

	bot->on = 0;

	bot_fini_subs(bot);

	if (bot->node_type == BOT_NODE_TYPE_SUB) {
		bot_fini(bot);
	}

	debug(NULL, "bot_turn_off: Cleaned\n");

	return;
}

void bot_exchange_data(bot_t * dst, bot_t * src)
{
	bot_t bot_inter;

	debug(src, "bot_exchange_data: Entered\n");

	if (!dst || !src)
		return;

	bz2(bot_inter);

	strlcpy_buf(bot_inter.irc_command, src->irc_command);
	strlcpy_buf(bot_inter.txt_nick, src->txt_nick);
	strlcpy_buf(bot_inter.txt_ident, src->txt_ident);
	strlcpy_buf(bot_inter.txt_to, src->txt_to);
	strlcpy_buf(bot_inter.txt_data_in, src->txt_data_in);
	strlcpy_buf(bot_inter.txt_data_out, src->txt_data_out);

	bot_inter.isprivmsg = src->isprivmsg;
	bot_inter.shouldsend = src->shouldsend;
	bot_inter.isline = src->isline;
	bot_inter.isforked = src->isforked;
	bot_inter.isforked_stop = src->isforked_stop;
	bot_inter.var_multi = src->var_multi;
	bot_inter.ispm = src->ispm;
	bot_inter.iscomment = src->iscomment;

	bot_line_clear(dst);

	strlcpy_buf(dst->irc_command, bot_inter.irc_command);
	strlcpy_buf(dst->txt_nick, bot_inter.txt_nick);
	strlcpy_buf(dst->txt_ident, bot_inter.txt_ident);
	strlcpy_buf(dst->txt_host, bot_inter.txt_host);
	strlcpy_buf(dst->txt_to, bot_inter.txt_to);
	strlcpy_buf(dst->txt_data_in, bot_inter.txt_data_in);
	strlcpy_buf(dst->txt_data_out, bot_inter.txt_data_out);

	dst->isprivmsg = bot_inter.isprivmsg;
	dst->shouldsend = bot_inter.shouldsend;
	dst->isline = bot_inter.isline;
	dst->isforked = bot_inter.isforked;
	dst->isforked_stop = bot_inter.isforked_stop;
	dst->var_multi = bot_inter.var_multi;
	dst->ispm = bot_inter.ispm;
	dst->iscomment = bot_inter.iscomment;

	return;
}

int change_server(bot_t * bot)
{

	debug(bot, "change_server: Entered\n");

	if (!bot)
		return (-1);

	return (0);
}

void bot_defaults(bot_t * bot)
{

	debug(bot, "bot_defaults: Entered\n");

	if (!bot)
		return;

	return;
}

dlist_t *bot_sub_add(bot_t * bot, bot_t * bot_sub)
{
	dlist_t *dptr_sub = NULL;
	debug(NULL, "bot_sub_add: Entered: %p %p\n", bot, bot_sub);

	if (!bot || !bot_sub)
		return NULL;

	dptr_sub = dlist_Dinsert_after(&bot->dl_subs, bot_sub);

	if (!dptr_sub) {
		return NULL;
	}

	bot_sub->dptr_self = dptr_sub;

	return dptr_sub;
}

bot_t *bot_dup_sub(bot_t * bot)
{
	bot_t *bot_sub = NULL;

	debug(bot, "bot_dup_sub: Entered\n");

	if (!bot)
		return (NULL);

	bot_sub = (bot_t *) calloc(1, sizeof(bot_t));
	if (!bot_sub)
		return NULL;

	memcpy(bot_sub, bot, sizeof(bot_t));

	bot_sub->dptr_self = NULL;
	bot_sub->parent = bot;
	bot_sub->dl_subs = NULL;
	bot_sub->node_type = BOT_NODE_TYPE_SUB;

	bot_sub->dl_module_arg = NULL;
	bot_sub->dl_module_help = NULL;
	bot_sub->dl_environ = NULL;

	bot_sub->dl_pmodules = NULL;
	bot_sub->dl_pmodules_cur = NULL;
	bot_sub->dl_control = NULL;

	bot_sub->ID = rand();

/* this needs to be independent of the sub */
	bot_sub->dl_gmodules = bot_gmodule_dup_bot_gmodules(bot);

	bot_sub->dl_gmodules_cur = NULL;

	return (bot_sub);
}

bot_t *bot_dup(bot_t * bot)
{
	bot_t *bot_copy = NULL;

	debug(bot, "bot_dup: Entered\n");

	if (!bot)
		return (NULL);

	bot_copy = (bot_t *) calloc(1, sizeof(bot_t));
	if (!bot_copy)
		return NULL;

	memcpy(bot_copy, bot, sizeof(bot_t));

	bot_copy->node_type = BOT_NODE_TYPE_ROOT;

	return (bot_copy);
}

bot_t *bot_find_tag(char *tag)
{
	dlist_t *dptr;
	bot_t *bot = NULL;

	debug(NULL, "bot_find_tag: Entered: tag=%s\n", tag);

	if (!_sNULL(tag))
		return NULL;

	dlist_fornext(gi->bots, dptr) {
		bot = (bot_t *) dlist_data(dptr);
		if (!bot)
			continue;

		if (!strcasecmp(bot->tag, tag)) {
			return bot;
		}

	}

	return NULL;
}

bot_t *bot_find_sub_by_tag(bot_t * the_bot, char *tag)
{
	dlist_t *dptr;
	bot_t *bot = NULL;

	debug(NULL, "bot_find_sub_by_tag: Entered: tag=%s\n", tag);

	if (!_sNULL(tag) || !the_bot)
		return NULL;

	dlist_fornext(the_bot->dl_subs, dptr) {
		bot = (bot_t *) dlist_data(dptr);
		if (!bot)
			continue;

		if (!strcasecmp(bot->tag, tag)) {
			return bot;
		}

	}

	return NULL;
}

bot_t *bot_find_sub_by_id(bot_t * the_bot, int id)
{
	dlist_t *dptr;
	bot_t *bot = NULL;

	debug(NULL, "bot_find_sub_by_id: Entered: id=%i\n", id);

	if (id <= 0 || !the_bot)
		return NULL;

	dlist_fornext(the_bot->dl_subs, dptr) {
		bot = (bot_t *) dlist_data(dptr);
		if (!bot)
			continue;

		if (bot->ID == id) {
			return bot;
		}

		if (bot->dl_subs) {
			bot = bot_find_sub_by_id(bot, id);
			if (bot)
				return bot;
		}

	}

	return NULL;
}

dlist_t **bot_find_all_dlist_by_bot_id(bot_t * bot, dlist_t ** node_ptr)
{
	dlist_t **dl_global, **dl_subs, *dptr_global, *dptr_subs;
	bot_t *global, *bot_sub;

	debug(NULL, "bot_find_all_dlist_by_bot_id: Entered\n");

	if (!bot || !node_ptr)
		return NULL;

	*node_ptr = NULL;

	dl_global = &gi->bots;
	dlist_fornext(*dl_global, dptr_global) {
		global = (bot_t *) dlist_data(dptr_global);
		if (!global)
			continue;

		if (global->ID == bot->ID) {
			*node_ptr = dptr_global;
			return dl_global;
		}

		dl_subs = &global->dl_subs;
		dlist_fornext(*dl_subs, dptr_subs) {
			bot_sub = (bot_t *) dlist_data(dptr_subs);

			if (bot_sub->ID == bot->ID) {
				*node_ptr = dptr_subs;
				return dl_subs;
			}

		}

	}

	return NULL;
}

bot_t *bot_find_all_by_id(int id)
{
	dlist_t **dl_global, **dl_subs, *dptr_global, *dptr_subs;
	bot_t *global, *bot_sub;

	debug(NULL, "bot_find_all_by_id: id=%i\n", id);

	if (id < 0)
		return NULL;

	dl_global = &gi->bots;
	dlist_fornext(*dl_global, dptr_global) {
		global = (bot_t *) dlist_data(dptr_global);
		if (!global)
			continue;

		if (global->ID == id) {
			return global;
		}

		dl_subs = &global->dl_subs;
		dlist_fornext(*dl_subs, dptr_subs) {
			bot_sub = (bot_t *) dlist_data(dptr_subs);

			if (bot_sub->ID == id) {
				return bot_sub;
			}

		}

	}

	return NULL;
}

void bot_fork_clean_exit(bot_t * bot)
{

	close(0);
	close(1);
	close(2);

	_exit(0);
	return;
}

pid_t bot_fork_clean(bot_t * bot)
{
	pid_t pid;

	debug(NULL, "bot_fork_clean: Entered\n");

	if (bot) {
		bot->isforked_stop = 1;
	}

	pid = fork();

	if (!pid) {

		gi->pid_child = getpid();

		event_reinit(gi->ev_base);

		close(0);

		if (bot) {
			bot->isforked_stop = 0;
			bot->isforked = 1;
		}

		global_signal_hooks_child();
	} else {
/* add pid to list of pids being controlled by this bot object */
		xpid_add(bot, pid);
	}

	return pid;
}

void
bot_copy_values(bot_t * bot, char *nick, char *ident, char *host,
		char *to, char *in, char *out)
{

	debug(NULL, "bot_copy_values: Entered\n");

	if (!bot)
		return;

	bot_line_clear(bot);

	if (_sNULL(nick)) {
		strlcpy_buf(bot->txt_nick, nick);
	}

	if (_sNULL(ident)) {
		strlcpy_buf(bot->txt_ident, ident);
	}

	if (_sNULL(host)) {
		strlcpy_buf(bot->txt_host, host);
	}

	if (_sNULL(to)) {
		strlcpy_buf(bot->txt_to, to);
	}

	if (_sNULL(in)) {
		strlcpy_buf(bot->txt_data_in, in);
		bot->txt_data_in_sz = strlen(in);
	}
	if (_sNULL(out)) {
		strlcpy_buf(bot->txt_data_out, out);
		bot->txt_data_out_sz = strlen(out);
	}

	return;
}

int
bot_daemon(int flags, char **mod_names, bot_t ** bots,
	   char *new_proc_name, char *fd_unix_path)
{
	dlist_t *dptr, *dptr_tmp;
	bot_t *bot_ptr;
	int i, found;
	pid_t pid;
	int nochdir = 0, noclose = 0;

	debug(NULL, "bot_daemon: Entered\n");

	if (flags & BOT_DAEMON_NOCHDIR) {
		nochdir = 1;
	}
	if (flags & BOT_DAEMON_NOCLOSE) {
		noclose = 1;
	}

	pid = xdaemon(nochdir, noclose);

	if (!pid) {

		if (flags & BOT_DAEMON_CLEAN_FDS) {
			for (i = 0; i < 65535; i++) {
				if (i == 1)
					continue;
				close(i);
			}
		}

		if (flags & BOT_DAEMON_EVENT_RESTART) {
			event_reinit(gi->ev_base);
		}

		if (flags & BOT_DAEMON_REMOVE_MODULES) {
			if (mod_names) {
				for (i = 0; _sNULL(mod_names[i]) != NULL; i++) {
					zmodule_lock_arg(mod_names[i]);
				}
				xmodule_unload_everything();
				zmodule_prune();
				for (i = 0; _sNULL(mod_names[i]) != NULL; i++) {
					zmodule_unlock_arg(mod_names[i]);
				}
			}
		}
		zmodule_list();

		if (flags & BOT_DAEMON_REMOVE_BOTS) {
			if (bots) {
				dlist_fornext_retarded(gi->bots, dptr, dptr_tmp) {
					if (!dptr)
						break;
					bot_ptr = (bot_t *) dlist_data(dptr);
					found = 0;

					for (i = 0; bots[i] != NULL; i++) {
						if (!strcasecmp
						    (bots[i]->tag,
						     bot_ptr->tag)) {
							found = 1;
						}
					}

					if (!found) {
						dlist_remove_and_free(&gi->bots,
								      &dptr,
								      (void (*)
								       (void *))
								      bot_turn_off);
					}

				}
			}
		}

		if (flags & BOT_DAEMON_RENAME_PROC) {
			xsetproctitle(new_proc_name);
		}

		if (fd_unix_path) {
			if (flags & BOT_DAEMON_UNIX_RESTART) {

				gi->fd_unix = 0;
				strlcpy_buf(gi->fd_unix_path, fd_unix_path);
				bot_unix_init();
			}
		}

	}

	return pid;
}
