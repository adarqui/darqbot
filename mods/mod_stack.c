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
#include "mod_stack.h"

void __stack_init__(void)
{

	strlcpy_buf(mod_stack_info.name, "mod_stack");
	strlcpy_buf(mod_stack_info.trigger, "^stack");

	module_add_subtrigs(&mod_stack_info, "^push");
	module_add_subtrigs(&mod_stack_info, "^pop");

	mod_stack_info.init = stack_init;
	mod_stack_info.fini = stack_fini;
	mod_stack_info.help = stack_help;
	mod_stack_info.run = stack_run;

	mod_stack_info.output = NULL;
	mod_stack_info.input = NULL;

	debug(NULL, "__stack_init__: Loaded mod_stack\n");

	return;
}

bot_t *stack_init(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "stack_init: Entered\n");

	swap_inmem_get_assign_and_remove("dl_mod_stack", 0,
					 (void **)&dl_mod_stack_unique);

	return NULL;
}

bot_t *stack_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "stack_fini: Entered\n");

	swap_inmem_create_and_add("dl_mod_stack", 0, dl_mod_stack_unique);

	return NULL;
}

bot_t *stack_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "stack_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^stack || ^stack(push:pop:clear:size:list) ...";

	return NULL;
}

bot_t *stack_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "stack_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "stack_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	if (!strcasecmp(bot->trig_called, "^push")) {
		opt = MOD_STACK_PUSH;
	} else if (!strcasecmp(bot->trig_called, "^pop")) {
		opt = MOD_STACK_POP;
	}

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "clear")) {
		opt = MOD_STACK_CLEAR;
	} else if (!strncasecmp_len(dl_options_ptr, "size")) {
		opt = MOD_STACK_SIZE;
	} else if (!strncasecmp_len(dl_options_ptr, "push")) {
		opt = MOD_STACK_PUSH;
	} else if (!strncasecmp_len(dl_options_ptr, "pop")) {
		opt = MOD_STACK_POP;
	} else if (!strncasecmp_len(dl_options_ptr, "list")) {
		opt = MOD_STACK_LIST;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = stack_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *stack_change_string(bot_t * bot, char *string, int opt)
{
	unique_t *bu = NULL;
	dlist_t **dl_mod_stack = NULL;
	dlist_t *dptr;
	char *str = NULL, *data_ptr;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	bz(buf);

	bu = unique_create(bot, &dl_mod_stack_unique,
			   UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
	if (!bu)
		return NULL;

	dl_mod_stack = (dlist_t **) & bu->data;

	if (opt == MOD_STACK_CLEAR) {
		dlist_fini(dl_mod_stack, free);
		return NULL;
	} else if (opt == MOD_STACK_SIZE) {
		strlcatfmt_buf(buf, "%i", dlist_size(*dl_mod_stack));
	} else if (opt == MOD_STACK_PUSH) {
		dlist_Dpush(dl_mod_stack, strdup(string));
	} else if (opt == MOD_STACK_POP) {
		dptr = dlist_pop(dl_mod_stack);
		if (!dptr)
			return NULL;
		if (!dlist_data(dptr))
			return NULL;
		return dlist_data(dptr);
	} else if (opt == MOD_STACK_LIST) {
		if (!dl_mod_stack)
			return NULL;
		if (dlist_size(*dl_mod_stack) == 0)
			return NULL;

		strlcatfmt_buf(buf, "[top]\n");
		dlist_fornext(*dl_mod_stack, dptr) {
			data_ptr = (char *)dlist_data(dptr);
			if (!data_ptr)
				continue;
			strlcatfmt_buf(buf, "%s\n", data_ptr);
		}
		strlcat_buf(buf, "[bottom]\n");
	}

	if (sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}
