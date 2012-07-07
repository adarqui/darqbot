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
#include "mod_rf.h"

void __rf_init__(void)
{

	strlcpy_buf(mod_rf_info.name, "mod_rf");
	strlcpy_buf(mod_rf_info.trigger, "^rf");

	mod_rf_info.init = rf_init;
	mod_rf_info.fini = rf_fini;
	mod_rf_info.help = rf_help;
	mod_rf_info.run = rf_run;

	mod_rf_info.output = NULL;
	mod_rf_info.input = NULL;

	debug(NULL, "__rf_init__: Loaded mod_rf\n");

	return;
}

bot_t *rf_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rf_init: Entered\n");

	return NULL;
}

bot_t *rf_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rf_fini: Entered\n");

	return NULL;
}

bot_t *rf_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rf_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^rf(func1(..):::func2:::func3(...))";

	return NULL;
}

bot_t *rf_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr;
	module_t *module;
	char *dl_module_arg_after_options, *dl_options_ptr;
	char **array, trig_tmp[32];
	int array_size;

	int rand_val;

	array = NULL;
	array_size = 0;

	debug(bot, "rf_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	if (gi->mongo_isconnected == 0) {
		debug(bot, "rf_run: Mongo is not connected\n");
		return NULL;
	}

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "rf_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	MOD_OPTIONS_TOP_HALF;

	array =
	    tokenize_array(bot, dl_options_ptr,
			   TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
			   ":::", &array_size);
	if (!array)
		return NULL;

	MOD_OPTIONS_BOTTOM_HALF;

	if (!array)
		return NULL;

	rand_val = rand() % (array_size);

	strlcpy_safe(&trig_tmp[1], ret_trig(&array[rand_val][1]),
		     sizeof(trig_tmp) - 2);
	if (!sNULL(&trig_tmp[1]))
		return NULL;

	trig_tmp[0] = '^';

	dptr = xmodule_find_by_trig_dptr(XMODULE_TYPE_MODULE, trig_tmp);
	if (dptr) {
		module = (module_t *) dlist_data(dptr);
		if (module) {
			if (module->run) {
				strlcpy_buf(bot->txt_data_in, array[rand_val]);
				bot->dl_module_arg =
				    bot->txt_data_in + strlen(module->trigger);
				module->run(dptr, bot);
			}
		}
	}

	tokenize_destroy_array(bot, array);

	return bot;
}
