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
#include "mod_console.h"

void __console_init__(void)
{

	strlcpy_buf(mod_console_info.name, "mod_console");
	strlcpy_buf(mod_console_info.trigger, "^console");

	mod_console_info.init = console_init;
	mod_console_info.fini = console_fini;
	mod_console_info.help = console_help;
	mod_console_info.run = console_run;

	mod_console_info.output = NULL;
	mod_console_info.input = NULL;

	debug(NULL, "__console_init__: Loaded mod_console\n");

	return;
}

bot_t *console_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "console_init: Entered\n");
	return NULL;
}

bot_t *console_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "console_fini: Entered\n");
	return NULL;
}

bot_t *console_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "console_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^console";

	return NULL;
}

bot_t *console_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "console_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "console_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = console_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *console_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char *sep_ptr;

	if (!string)
		return NULL;

	debug(NULL, "console_change_string: Entere: string=%s, opt=%i\n",
	      string, opt);

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	bot_dl_print_flush(bot);
	console_parse(bot, string);

	bot_dl_print_change_strings(bot, "\t", " ");
	str = bot_dl_print_flush_str(bot);

	return str;
}
