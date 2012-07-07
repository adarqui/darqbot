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
#include "mod_backtrace.h"

void __backtrace_init__(void)
{

	strlcpy_buf(mod_backtrace_info.name, "mod_backtrace");
	strlcpy_buf(mod_backtrace_info.trigger, "^backtrace");

	mod_backtrace_info.init = backtrace_init;
	mod_backtrace_info.fini = backtrace_fini;
	mod_backtrace_info.help = backtrace_help;
	mod_backtrace_info.run = backtrace_run;

	mod_backtrace_info.output = NULL;
	mod_backtrace_info.input = NULL;

	debug(NULL, "__backtrace_init__: Loaded mod_backtrace\n");

	return;
}

bot_t *backtrace_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "backtrace_init: Entered\n");
	return NULL;
}

bot_t *backtrace_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "backtrace_fini: Entered\n");
	return NULL;
}

bot_t *backtrace_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "backtrace_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^backtrace || ^backtrace(num)";

	return NULL;
}

bot_t *backtrace_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "backtrace_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "backtrace_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 7;

	MOD_OPTIONS_TOP_HALF;
	opt = atoi(dl_options_ptr);
	MOD_OPTIONS_BOTTOM_HALF;

	if (opt <= 0)
		opt = 1;

	MOD_PARSE_TOP_HALF_VOID;
	l_new_str = backtrace_change_string(l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_VOID;

	return bot;
}

char *backtrace_change_string(char *string, int opt)
{
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	void *bt_array[opt + 1];
	char **bt_strings = NULL;
	size_t bt_size;

	int i;

	if (opt <= 0)
		return NULL;

	memset(buf, 0, sizeof(buf));

	memset(bt_array, 0, (opt + 1) * sizeof(void *));
	bt_size = backtrace(bt_array, opt);

	bt_strings = backtrace_symbols(bt_array, bt_size);
	if (!bt_strings)
		goto cleanup;

	for (i = 0; i < bt_size; i++) {
		printf("bt_strings[%i]=%s\n", i, bt_strings[i]);
		strlcatfmt_bot(buf, "%s\n", bt_strings[i]);
	}

 cleanup:

	if (sNULL(buf) != NULL)
		str = strdup(buf);

	if (bt_strings)
		free(bt_strings);

	return str;
}
