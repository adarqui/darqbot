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

void console_info(bot_t * bot, char *arg)
{
	bot_t *bot_ptr;
	int all = 1;

	if (!bot || !arg)
		return;

	_strstrip_nl(arg);

	debug(bot, "console_info: Entered, arg=[%s]\n", arg);

	if (_strlen(arg))
		all = 0;

	bot_ptr = bot_find_tag(arg);

	if (bot_ptr)
		console_info_print(bot_ptr);

	return;
}

void console_info_print(bot_t * bot)
{
	dlist_t *dptr;

	if (!bot)
		return;

	bot_dl_print(bot, "printing out config info for bot=%s:\n", bot->tag);

	bot_dl_print(bot, "\tID: %i\n", bot->ID);

	bot_dl_print(bot, "\tstatus: %s\n", bot->on ? "on" : "off");

	bot_dl_print(bot,
		     "\ttrace: %i , debug: %i => gd->trace=%i , gd->debug=%i\n",
		     bot->trace, bot->debug, gd->trace, gd->debug);

	bot_dl_print(bot, "\tservers:\n");

	bot_dl_print(bot, "\tnicks:\n");

	bot_dl_print(bot, "\tchannels:\n");

	if (bot->logfile)
		bot_dl_print(bot, "\tlogfile=%s\n", bot->logfile);

	bot_dl_print(bot,
		     "\tvars: multi=%i, usleep=%i, maxbufsz=%i, bufsz=%i, maxflood=%i, maxnesting=%i, nesting=%i, nesting_cur=%i, allowpm=%i\n",
		     bot->var_multi, bot->var_usleep, bot->var_maxbufsz,
		     bot->var_bufsz, bot->var_maxflood, bot->var_maxnesting,
		     bot->var_nesting, bot->var_nesting_cur, bot->var_allowpm);

	if (bot->dl_subs) {
		bot_dl_print(bot, "\tsubs: %i\n", dlist_size(bot->dl_subs));
	}

	if (bot->dl_gmodules) {
		bot_gmod_elm_t *gmod;
		bot_dl_print(bot, "\tgmodules: %i\n",
			     dlist_size(bot->dl_gmodules));
		dlist_fornext(bot->dl_gmodules, dptr) {
			gmod = (bot_gmod_elm_t *) dlist_data(dptr);
			bot_dl_print(bot, "\t\t%s\n", gmod->syntax);
		}
	}

	return;
}

void console_tag_parse(bot_t * bot, char *arg)
{
	char *str;

	if (!bot || !arg)
		return;

	_strstrip_nl(arg);

	debug(bot, "console_tag_parse: Entered, arg=[%s]\n", arg);

	if (!strncasecmp_len(arg, "info")) {
		console_info_print(bot);
	} else if (!strncasecmp_len(arg, "swapiminfo")) {
		swap_inmem_console_info(bot);
	} else if (!strncasecmp_len(arg, "on")) {
		bot_turn_on(bot, 1);
	} else if (!strncasecmp_len(arg, "off")) {
		bot_turn_off(bot);
	} else if (!strncasecmp_len(arg, "reload")) {

	} else if (!strncasecmp_len(arg, "kill")) {
		bot_fini(bot);
	} else if (!strncasecmp_len(arg, "mute")) {
		bot->mute = 1;
	} else if (!strncasecmp_len(arg, "unmute")) {
		bot->mute = 0;
	} else if (!strncasecmp_len(arg, "raw")) {
		str = &arg[4];
	}

	else if (!strncasecmp_len(arg, "trace")) {
		bot_parse_trace(bot, &arg[_strlen("trace") + 1], 1);
	} else if (!strncasecmp_len(arg, "debug")) {
		bot_parse_debug(bot, &arg[_strlen("debug") + 1], 1);
	} else if (!strncasecmp_len(arg, "pid")) {
		xpid_console_print(bot, bot->tag);
	} else if (!strncasecmp_len(arg, "brake")) {
		char *l = &arg[_strlen("brake") + 1];
		if (!strcasecmp(l, "on")) {
			bot->brake = 1;
		} else if (!strcasecmp(l, "off")) {
			bot->brake = 0;
		}

	}

	return;
}

int bot_help_console(bot_t * bot)
{

	if (!bot)
		return (-1);

	bot_dl_print(bot,
		     "darqbot console help: <command> args..\n"
		     "\tmodload, modunload, modreload, modlist, modlock, modunlock ::: ...\n"
		     "\tpmodload, pmodunload, pmodreload, pmodlist, pmodlock, pmodunlock ::: ...\n"
		     "\tgmodload, gmodunload, gmodreload, gmodlist, gmodlock, gmodunlock ::: ...\n"
		     "\tzmodload, zmodunload, zmodreload, zmodlist, zmodlock, zmodunlock ::: ...\n"
		     "\tmodiolist, gmodiolist, zmodiolist ::: ...\n"
		     "\tmodtimerlist, gmodtimerlist, zmodtimerlist ::: ...\n"
		     "\ttestmem ::: ...\n"
		     "\ttestfd ::: ...\n"
		     "\tclose ::: ...\n"
		     "\ttrace ::: ...\n"
		     "\tdebug ::: ...\n"
		     "\tpid ::: ...\n"
		     "\tprint_all ::: ...\n"
		     "\tinfo ::: ...\n"
		     "\texiti, quit ::: ...\n"
		     "\toff ::: ...\n" "\ton ::: ...\n" "\thelp ::: ...\n");

	bot_dl_print(bot,
		     "darqbot console help: <tag> <command> args..\n"
		     "\tinfo ::: ...\n"
		     "\traw <text> ::: sends <text> directly over fd_link\n"
		     "\tswapiminfo ::: ...\n"
		     "\ton ::: ...\n"
		     "\toff ::: ...\n"
		     "\treload ::: ...\n"
		     "\tkill ::: ...\n"
		     "\tmute ::: ...\n"
		     "\tunmute ::: ...\n" "\ttrace ::: ...\n"
		     "\tdebug ::: ...\n" "\tbreak ::: ...\n");

	return (0);
}

void bot_evhook_console(int fd, short event, void *arg)
{
/* libevent hook for our console, ie, stdin for "superuser" control of the bot */
	bot_t *bot = NULL;
	int n;
	char buf[MAX_BUF_SZ + 1];

	int bot_set = 0;

	if (global_info.bots) {
		bot = dlist_data(global_info.bots);
		bot_set = 1;
	} else
		bot = NULL;

	debug(NULL, "bot_evhook_console: Entered\n");

	bz(buf);

	n = read(fd, buf, sizeof(buf) - 1);
	if (n <= 0) {
		debug(bot, "bot_evhook_console: read: %s\n", strerror(errno));
		bot_unset_evhooks(bot);
		return;
	}

	buf[n - 1] = '\0';

	debug(bot, "bot_evhook_console: read [%s]\n", buf);

	bot_dl_print_flush(bot);
	console_parse(bot, buf);
	bot_dl_print_flush(bot);

	return;

}

void console_parse(bot_t * bot, char *buf)
{
	int bot_set = 0;
	char *tok;

	debug(NULL, "console_parse: Entered\n");

	if (bot)
		bot_set = 1;

	if (!strncasecmp_len(buf, "modload ")) {

		console_modload(&buf[_strlen("modload ")]);
	} else if (!strncasecmp_len(buf, "modunload ")) {
		console_modunload(&buf[_strlen("modunload ")]);
	} else if (!strncasecmp_len(buf, "modreload ")) {
		console_modreload(&buf[_strlen("modreload ")]);
	} else if (!strncasecmp_len(buf, "modlock ")) {
		module_lock(&buf[_strlen("modlock ")]);
	} else if (!strncasecmp_len(buf, "modunlock ")) {
		module_unlock(&buf[_strlen("modunlock ")]);
	} else if (!strncasecmp_len(buf, "modlist")) {
		module_list();
	} else if (!strncasecmp_len(buf, "pmodload ")) {
		console_pmodload(&buf[_strlen("pmodload ")]);
	} else if (!strncasecmp_len(buf, "pmodunload ")) {
		console_pmodunload(&buf[_strlen("pmodunload ")]);
	} else if (!strncasecmp_len(buf, "pmodreload ")) {
		console_pmodreload(&buf[_strlen("pmodreload ")]);
	} else if (!strncasecmp_len(buf, "pmodlock ")) {
		pmodule_lock(&buf[_strlen("pmodlock ")]);
	} else if (!strncasecmp_len(buf, "pmodunlock ")) {
		pmodule_unlock(&buf[_strlen("pmodunlock ")]);
	} else if (!strncasecmp_len(buf, "pmodlist")) {
		pmodule_list();
	} else if (!strncasecmp_len(buf, "gmodload ")) {
		console_gmodload(&buf[_strlen("gmodload ")]);
	} else if (!strncasecmp_len(buf, "gmodunload ")) {
		console_gmodunload(&buf[_strlen("gmodunload ")]);
	} else if (!strncasecmp_len(buf, "gmodreload ")) {
		console_gmodreload(&buf[_strlen("gmodreload ")]);
	} else if (!strncasecmp_len(buf, "gmodlock ")) {
		gmodule_lock(&buf[_strlen("gmodlock ")]);
	} else if (!strncasecmp_len(buf, "gmodunlock ")) {
		gmodule_unlock(&buf[_strlen("gmodunlock ")]);
	} else if (!strncasecmp_len(buf, "gmodlist")) {
		gmodule_list();
	} else if (!strncasecmp_len(buf, "zmodload")) {
		console_zmodload(&buf[_strlen("zmodload")]);
	} else if (!strncasecmp_len(buf, "zmodunload")) {
		console_zmodunload(&buf[_strlen("zmodunload")]);
	} else if (!strncasecmp_len(buf, "zmodreload")) {
		console_zmodreload(&buf[_strlen("zmodreload")]);
	} else if (!strncasecmp_len(buf, "zmodlock")) {
		zmodule_lock();
	} else if (!strncasecmp_len(buf, "zmodunlock")) {
		zmodule_unlock();
	} else if (!strncasecmp_len(buf, "zmodlist")) {
		zmodule_list();
	} else if (!strncasecmp_len(buf, "modiolist")) {
		module_iolist();
	} else if (!strncasecmp_len(buf, "gmodiolist")) {
		gmodule_iolist();
	} else if (!strncasecmp_len(buf, "zmodiolist")) {
		zmodule_iolist();
	} else if (!strncasecmp_len(buf, "modtimerlist")) {
		module_timerlist();
	} else if (!strncasecmp_len(buf, "gmodtimerlist")) {
		gmodule_timerlist();
	} else if (!strncasecmp_len(buf, "zmodtimerlist")) {
		zmodule_timerlist();
	}

	else if (!strncasecmp_len(buf, "testmem")) {
		unsigned char *memptr;

		memptr = calloc(1, 1024);

/* just dont access these ptr's */
		if (g_test_mem_1)
			g_test_mem_2 = g_test_mem_1;
		g_test_mem_1 = memptr;

		bot_dl_print(bot,
			     "TESTMEM: g_test_mem_1=%p, g_test_mem_2=%p, diff(1:2)=%i ::: g_test_mem_3=%p diff(1:3)=%i\n",
			     g_test_mem_1, g_test_mem_2,
			     g_test_mem_1 >
			     g_test_mem_2 ? g_test_mem_1 -
			     g_test_mem_2 : g_test_mem_2 - g_test_mem_1,
			     g_test_mem_3,
			     g_test_mem_1 >
			     g_test_mem_3 ? g_test_mem_1 -
			     g_test_mem_3 : g_test_mem_3 - g_test_mem_1);

		if (memptr)
			free(memptr);

	} else if (!strncasecmp_len(buf, "testfd")) {
		int tmpfd;

		tmpfd = open("/tmp/", O_RDONLY);

		if (g_test_fd_1)
			g_test_fd_2 = g_test_fd_1;
		g_test_fd_1 = tmpfd;

		bot_dl_print(bot,
			     "TESTFD: g_test_fd_1=%i, g_test_fd_2=%i, diff(1:2)=%i ::: g_test_fd_3=%i diff(1:3)=%i\n",
			     g_test_fd_1, g_test_fd_2,
			     g_test_fd_1 >
			     g_test_fd_2 ? g_test_fd_1 -
			     g_test_fd_2 : g_test_fd_2 - g_test_fd_1,
			     g_test_fd_3,
			     g_test_fd_1 >
			     g_test_fd_3 ? g_test_fd_1 -
			     g_test_fd_3 : g_test_fd_3 - g_test_fd_1);

		if (tmpfd)
			safe_close(tmpfd);
	}

	else if (!strncasecmp_len(buf, "close")) {
		int fd_to_close = 0;

		fd_to_close = atoi(&buf[_strlen("close") + 1]);
		debug(bot, "console_parse: Closing fd=%i\n", fd_to_close);
		safe_close(fd_to_close);
	} else if (!strncasecmp_len(buf, "trace")) {
		global_parse_trace(&buf[_strlen("trace") + 1], 1);
	} else if (!strncasecmp_len(buf, "debug")) {
		global_parse_debug(&buf[_strlen("debug") + 1], 1);
	} else if (!strncasecmp_len(buf, "pid")) {
		xpid_console_print(bot, NULL);
	} else if (!strncasecmp_len(buf, "print_all")) {
		console_print_all(0);
	} else if (!strncasecmp_len(buf, "info")) {
		console_info(bot, &buf[_strlen("info") + 1]);
	} else if (!strncasecmp_len(buf, "exit")) {
/* purge children etc.. */
		gi->atexit_code = 254;
		exit(gi->atexit_code);
	} else if (!strncasecmp_len(buf, "quit")) {
/* purge children etc */
		gi->atexit_code = 254;
		exit(gi->atexit_code);
	} else if (!strncasecmp_len(buf, "off")) {
		global_off();
	} else if (!strncasecmp_len(buf, "on")) {
		global_on();
	} else if (!strncasecmp_len(buf, "help")) {
		bot_help_console(bot);
	} else {
/* this is probably a tag */
		bot_t *bot_tmp;

		char *tag = NULL, *rest = NULL;
		tok = _strchr(buf, ' ');
		if (!tok)
			goto cleanup;

		tag = strdup_len(buf, (tok - buf));
		bot_tmp = bot_find_tag(tag);

		if (bot_tmp) {
			console_tag_parse(bot_tmp, &buf[_strlen(tag) + 1]);
		} else {
/* not found, but if second argument is "on", we need to create & load it */
			rest = tok + 1;
			if (_sNULL(rest)) {
				if (!strncasecmp_len(rest, "on")) {
					debug(NULL,
					      "console_parse: Attempting to load %s\n",
					      tag);
					bot_init_and_turn_on(tag);
				}
			}
		}

		free(tag);

	}

 cleanup:

	return;
}

void console_print_all(int tab)
{
	dlist_t *dptr = NULL;
	bot_t *bot;

	debug(NULL, "console_print_all: Entered\n");
	if (tab < 0)
		tab = 0;

	dlist_fornext(gi->bots, dptr) {
		bot = (bot_t *) dlist_data(dptr);
		console_print_all_bot(bot, tab);
	}

	return;
}

void console_print_all_bot(bot_t * bot, int tab)
{
	dlist_t *dptr = NULL;
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "console_print_all_bot: Entered, tag=%s\n", bot->tag);

	if (!bot)
		return;

	bot_dl_print(bot, "    \n%sbot: %p tag=%s, ID=%i, type=%i\n",
		     str_create_spaces(tab), bot, bot->tag, bot->ID, bot->type);

	tab = tab + 1;

	if (tab >= 0) {
		dlist_fornext(bot->dl_gmodules, dptr) {

			gmod = (bot_gmod_elm_t *) dlist_data(dptr);

			if (!gmod) {
				bot_dl_print(bot, "%sgmod: NULL\n",
					     str_create_spaces(tab + 1));
			} else {

/*
	    bot_dl_print (bot, "%sgmod = %p\n", str_create_spaces (tab + 1),
			  gmod);
	    bot_dl_print (bot,
			  "%sgmod: index=%i, has_run=%i, can_pass_up=%i, can_pass_down=%i\n",
			  str_create_spaces (tab + 1), gmod->index,
			  gmod->has_run, gmod->can_pass_up,
			  gmod->can_pass_down);
	    bot_dl_print (bot, "%sbot=%p, trigger=%s, trigger_ext=%s\n",
			  str_create_spaces (tab + 1), gmod->bot,
			  gmod->trigger, gmod->trigger_ext);
	    bot_dl_print (bot, "%ssyntax=%s\n", str_create_spaces (tab + 1),
			  gmod->syntax);
	    bot_dl_print (bot, "%sdata_type=%i, data=%p, dl_cache=%i\n",
			  str_create_spaces (tab + 1), gmod->data_type,
			  gmod->data, dlist_size (gmod->dl_cache));
*/
			}
		}
	}

	dlist_fornext(bot->dl_subs, dptr) {

		bot = (bot_t *) dlist_data(dptr);
		console_print_all_bot(bot, -1);
	}

	return;
}
