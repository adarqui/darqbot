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
#include "mod_fork.h"

void __fork_init__(void)
{

	strlcpy_buf(mod_fork_info.name, "mod_fork");
	strlcpy_buf(mod_fork_info.trigger, "^fork");

	mod_fork_info.init = fork_init;
	mod_fork_info.fini = fork_fini;
	mod_fork_info.help = fork_help;
	mod_fork_info.run = fork_run;

	mod_fork_info.output = NULL;
	mod_fork_info.input = NULL;

	debug(NULL, "__fork_init__: Loaded mod_fork\n");

	return;
}

bot_t *fork_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "fork_init: Entered\n");
	return NULL;
}

bot_t *fork_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "fork_fini: Entered\n");
	return NULL;
}

bot_t *fork_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "fork_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^fork || ^fork(-v:-d)";

	return NULL;
}

bot_t *fork_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	char **argv = NULL;
	int argc = 0;
	int c, opt;

	debug(bot, "fork_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	debug(bot,
	      "fork_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	stat_inc(bot, bot->trig_called);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	argv =
	    tokenize_str2argv(dl_options_ptr, &argc, TOKENIZE_STR2ARGV_ARGV0);
	if (argv) {

		optind = 0;

		while ((c = getopt(argc, argv, "vd")) != -1) {
			switch (c) {
			case 'v':
				{
					opt |= MOD_FORK_OPT_VERBOSE;
					break;
				}
			case 'd':
				{
					opt |= MOD_FORK_OPT_DAEMON;
					break;
				}
			default:
				break;
			}

		}

		tokenize_destroy_array(NULL, argv);
	}

	MOD_OPTIONS_BOTTOM_HALF;

	bot->shouldsend = 1;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = fork_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *fork_change_string(bot_t * bot, char *string, int opt)
{
	pid_t pid_new, pid_orig;
	char *sep_ptr;
	char *str = NULL;

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	pid_orig = getpid();

	pid_new = bot_fork_clean(bot);

	debug(NULL, "fork_change_string: pid=%i\n", pid_new);

	if (!pid_new && (opt & MOD_FORK_OPT_VERBOSE)) {
		pid_new = getpid();
		str = str_unite("{pid_orig=%i, pid_new=%i}", pid_orig, pid_new);
	}

	return str;
}
