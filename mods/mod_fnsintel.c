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
#include "mod_fnsintel.h"

void __fnsintel_init__(void)
{

	strlcpy_buf(mod_fnsintel_info.name, "mod_fnsintel");
	strlcpy_buf(mod_fnsintel_info.trigger, "^fnsintel");

	mod_fnsintel_info.init = fnsintel_init;
	mod_fnsintel_info.fini = fnsintel_fini;
	mod_fnsintel_info.help = fnsintel_help;
	mod_fnsintel_info.run = fnsintel_run;

	mod_fnsintel_info.output = NULL;
	mod_fnsintel_info.input = NULL;

	debug(NULL, "__fnsintel_init__: Loaded mod_fnsintel\n");

	return;
}

bot_t *fnsintel_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "fnsintel_init: Entered\n");
	return NULL;
}

bot_t *fnsintel_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "fnsintel_fini: Entered\n");

	fnsintel_off(bot);

	return NULL;
}

bot_t *fnsintel_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "fnsintel_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^fnsintel";

	return NULL;
}

bot_t *fnsintel_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "fnsintel_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "fnsintel_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;
	opt = MOD_FNSINTEL_TEST;

	MOD_OPTIONS_TOP_HALF;

	if (!strcasecmp(dl_options_ptr, "test")) {
		opt = MOD_FNSINTEL_TEST;
	} else if (!strcasecmp(dl_options_ptr, "on")) {
		opt = MOD_FNSINTEL_ON;
	} else if (!strcasecmp(dl_options_ptr, "off")) {
		opt = MOD_FNSINTEL_OFF;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = fnsintel_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *fnsintel_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char *sep_ptr;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (opt == MOD_FNSINTEL_TEST) {
		str = fnsintel_test(bot);
	} else if (opt == MOD_FNSINTEL_ON) {
		fnsintel_on(bot);
	} else if (opt == MOD_FNSINTEL_OFF) {
		fnsintel_off(bot);
	}

	return str;
}

void fnsintel_on(bot_t * bot)
{

	debug(bot, "fnsintel_on: Entered\n");

/*
	fn_true = asmx86_true;
	fn_false = asmx86_false;
	fn_val_int = asmx86_val_int;
	fn_val_string = asmx86_val_string;
	fn_strlen = asmx86_strlen;
	fn_memset = asmx86_memset;
	fn_bzero = asmx86_bzero;
	fn_strcmp = asmx86_strcmp;
	fn_strncmp = asmx86_strncmp;
*/

	return;
}

void fnsintel_off(bot_t * bot)
{

	debug(bot, "fnsintel_off: Entered\n");

	fns_load_defaults();

	return;
}

char *fnsintel_test(bot_t * bot)
{

	return NULL;
}
