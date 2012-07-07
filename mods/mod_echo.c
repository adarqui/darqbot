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
#include "mod_echo.h"

void __echo_init__(void)
{

	strlcpy_buf(mod_echo_info.name, "mod_echo");
	strlcpy_buf(mod_echo_info.trigger, "^echo");

	module_add_subtrigs(&mod_echo_info, "^e");
	module_add_subtrigs(&mod_echo_info, "^say");

	mod_echo_info.init = echo_init;
	mod_echo_info.fini = echo_fini;
	mod_echo_info.help = echo_help;
	mod_echo_info.run = echo_run;

	mod_echo_info.output = NULL;
	mod_echo_info.input = NULL;

	debug(NULL, "__echo_init__: Loaded mod_echo\n");

	return;
}

bot_t *echo_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "echo_init: Entered\n");
	return NULL;
}

bot_t *echo_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "echo_fini: Entered\n");
	return NULL;
}

bot_t *echo_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "echo_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^echo";

	return NULL;
}

bot_t *echo_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "echo_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	debug(bot,
	      "echo_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	stat_inc(bot, bot->trig_called);

	if (bot_shouldreturn(bot))
		return NULL;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	bot->shouldsend = 1;

	MOD_PARSE_TOP_HALF;
	l_new_str = echo_change_string(l_str_ptr);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *echo_change_string(char *string)
{
	char *sep_ptr;

	char *str = NULL;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	str = strdup(string);

	return str;
}
