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
#include "mod_packmulti.h"

void __packmulti_init__(void)
{

	strlcpy_buf(mod_packmulti_info.name, "mod_packmulti");
	strlcpy_buf(mod_packmulti_info.trigger, "^packmulti");

	mod_packmulti_info.init = packmulti_init;
	mod_packmulti_info.fini = packmulti_fini;
	mod_packmulti_info.help = packmulti_help;
	mod_packmulti_info.run = packmulti_run;

	mod_packmulti_info.output = NULL;
	mod_packmulti_info.input = NULL;

	debug(NULL, "__packmulti_init__: Loaded mod_packmulti\n");

	return;
}

bot_t *packmulti_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "packmulti_init: Entered\n");
	return NULL;
}

bot_t *packmulti_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "packmulti_fini: Entered\n");
	return NULL;
}

bot_t *packmulti_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "packmulti_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^packmulti(num)";

	return NULL;
}

bot_t *packmulti_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot, "packmulti_run: Entered\n");

	debug(bot,
	      "packmulti_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	opt = atoi(dl_options_ptr);
	MOD_OPTIONS_BOTTOM_HALF;

	if (opt > bot->var_bufsz)
		opt = bot->var_bufsz;

	if (opt == 0)
		opt = bot->var_bufsz;

	MOD_PARSE_TOP_HALF;
	l_new_str = packmulti_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *packmulti_change_string(bot_t * bot, char *string, int opt)
{
	int i;
	char buf[MAX_BUF_SZ + 1];

	char *sep_ptr;
	char *str = NULL;

	if (!bot || !string)
		return 0;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	bz(buf);

	i = 0;
	while (*string) {

		if (i == opt) {
			i = 0;
			charcat_safe(buf, '\n', sizeof(buf) - 1);
		}

		charcat_safe(buf, *string, sizeof(buf) - 1);
		string++;
		i++;
	}

	if (sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}
