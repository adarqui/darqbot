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
#include "mod_var.h"

void __var_init__(void)
{

	strlcpy_buf(mod_var_info.name, "mod_var");
	strlcpy_buf(mod_var_info.trigger, "^var");

	mod_var_info.init = var_init;
	mod_var_info.fini = var_fini;
	mod_var_info.help = var_help;
	mod_var_info.run = var_run;

	mod_var_info.output = NULL;
	mod_var_info.input = NULL;

	debug(NULL, "__var_init__: Loaded mod_var\n");

	return;
}

bot_t *var_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "var_init: Entered\n");
	return NULL;
}

bot_t *var_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "var_fini: Entered\n");

	return NULL;
}

bot_t *var_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "var_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^var(usleep:bufsz:multi:maxflood:nesting_cur:allowpm:sigprotect:timer_sleep:timer_sort)";

	return NULL;
}

bot_t *var_run(dlist_t * dlist_node, bot_t * bot)
{
	char *l_str_ptr = NULL, *l_new_str =
	    NULL, *dl_module_arg_after_options, *dl_options_ptr;
	char *(*optfn) (bot_t *, char *);

	debug(bot, "var_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "var_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	optfn = NULL;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "usleep")) {
		optfn = var_usleep_fn;
	} else if (!strncasecmp_len(dl_options_ptr, "multi")) {
		optfn = var_multi_fn;
	} else if (!strncasecmp_len(dl_options_ptr, "bufsz")) {
		optfn = var_bufsz_fn;

	} else if (!strncasecmp_len(dl_options_ptr, "maxflood")) {
		optfn = var_maxflood_fn;
	} else if (!strncasecmp_len(dl_options_ptr, "nesting_cur")) {
		optfn = var_nesting_cur_fn;
	}

	else if (!strncasecmp_len(dl_options_ptr, "nesting")) {
		optfn = var_nesting_fn;
	}

	else if (!strncasecmp_len(dl_options_ptr, "allowpm")) {
		optfn = var_allowpm_fn;
	} else if (!strncasecmp_len(dl_options_ptr, "sigprotect")) {
		optfn = var_sigprotect_fn;
	} else if (!strncasecmp_len(dl_options_ptr, "timer_sleep")) {
		optfn = var_timer_sleep_fn;
	} else if (!strncasecmp_len(dl_options_ptr, "timer_sort")) {
		optfn = var_timer_sort_fn;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	l_str_ptr = _eat_whitespace(dl_module_arg_after_options);
	if (!_sNULL(l_str_ptr))
		l_str_ptr = NULL;

	if (optfn) {
		l_new_str = optfn(bot, l_str_ptr);
		if (l_new_str) {
			strlcat_bot(bot->txt_data_out, l_new_str);
			free(l_new_str);
		}
	}

	return bot;
}

char *var_usleep_fn(bot_t * bot, char *str)
{
	int val;
	if (!bot)
		return NULL;

	debug(bot, "var_usleep_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(bot->var_usleep);
	}

	val = atoi(str);
	if (val == 0)
		return NULL;

	if (val < 0)
		val = val * -1;
	bot->var_usleep = val;
	return 0;
}

char *var_multi_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_multi_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(bot->var_multi);
	}

	val = atoi(str);
	if (val < 0)
		val = val * -1;
	bot->var_multi = val;
	return NULL;
}

char *var_bufsz_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_bufsz_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(bot->var_bufsz);
	}

	val = atoi(str);
	if (val < 0)
		val = val * -1;

	if (val > bot->var_maxbufsz)
		val = bot->var_maxbufsz;

	if (val > MAX_BUF_SZ)
		val = MAX_BUF_SZ - 1;

	if (val == 0)
		val = 1;

	bot->var_bufsz = val;
	return NULL;
}

char *var_maxflood_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_maxflood_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(bot->var_maxflood);
	}

	val = atoi(str);
	if (val < 0)
		val = val * -1;

	if (val == 0)
		val = 1;

	bot->var_maxflood = val;
	return NULL;
}

char *var_nesting_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_nesting_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(bot->var_nesting);
	}

	val = atoi(str);
	if (val < 0)
		val = val * -1;

	if (val > bot->var_maxnesting)
		val = bot->var_maxnesting;

	if (val == 0)
		val = 1;

	bot->var_nesting = val;
	return NULL;
}

char *var_nesting_cur_fn(bot_t * bot, char *str)
{

	if (!bot)
		return NULL;

	debug(bot, "var_nesting_cur_fn: Entered, str=%s\n", str);

	return strdup_int(bot->var_nesting_cur);
}

char *var_allowpm_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_allowpm_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(bot->var_allowpm);
	}

	val = atoi(str);
	if (val < 0)
		val = 0;

	if (val >= 1)
		val = 1;

	bot->var_allowpm = val;
	return NULL;
}

char *var_sigprotect_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_sigprotect_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(gi->var_sigprotect);
	}

	if (!strcasecmp(str, "ON")) {
		val = 1;
	} else if (!strcasecmp(str, "off")) {
		val = 0;
	} else
		return NULL;

	gi->var_sigprotect = val;
	return NULL;
}

char *var_timer_sleep_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_timer_sleep_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(gi->var_timer_sleep);
	}

	val = atoi(str);
	if (val <= 0)
		val = 30;

	gi->var_timer_sleep = val;
	return NULL;
}

char *var_timer_sort_fn(bot_t * bot, char *str)
{
	int val;

	if (!bot)
		return NULL;

	debug(bot, "var_timer_sort_fn: Entered, str=%s\n", str);

	if (!str) {
		return strdup_int(gi->var_timer_sort);
	}

	val = atoi(str);
	if (val <= 0)
		val = 600;

	gi->var_timer_sort = val;
	return NULL;
}
