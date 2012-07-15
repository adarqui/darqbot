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
#include "mod_rant.h"

void __rant_init__(void)
{

	strlcpy_buf(mod_rant_info.name, "mod_rant");
	strlcpy_buf(mod_rant_info.trigger, "^rant");

	mod_rant_info.init = rant_init;
	mod_rant_info.fini = rant_fini;
	mod_rant_info.help = rant_help;
	mod_rant_info.run = rant_run;

	mod_rant_info.output = NULL;
	mod_rant_info.input = rant_input;

	debug(NULL, "__rant_init__: Loaded mod_rant\n");

	return;
}

bot_t *rant_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rant_init: Entered\n");

	swap_inmem_get_assign_and_remove("dl_mod_rant", 0,
					 (void **)&dl_mod_rant_unique);

	return NULL;
}

bot_t *rant_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rant_fini: Entered\n");

	swap_inmem_create_and_add("dl_mod_rant", 0, dl_mod_rant_unique);

	return NULL;
}

bot_t *rant_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rant_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^rant || ^rant(clear:size:push:pop:list) ...";

	return NULL;
}

bot_t *rant_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "rant_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "rant_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	opt |= MOD_RANT_PUSH;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "clear")) {
		opt &= ~MOD_RANT_PUSH;
		opt = MOD_RANT_CLEAR;
	} else if (!strncasecmp_len(dl_options_ptr, "size")) {
		opt &= ~MOD_RANT_PUSH;
		opt = MOD_RANT_SIZE;
	} else if (!strncasecmp_len(dl_options_ptr, "push")) {
		opt = MOD_RANT_PUSH;
	} else if (!strncasecmp_len(dl_options_ptr, "pop")) {
		opt &= ~MOD_RANT_PUSH;
		opt = MOD_RANT_POP;
	} else if (!strncasecmp_len(dl_options_ptr, "list")) {
		opt &= ~MOD_RANT_PUSH;
		opt = MOD_RANT_LIST;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = rant_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *rant_change_string(bot_t * bot, char *string, int opt)
{
	unique_t *bu = NULL;
	dlist_t **dl_mod_rant = NULL;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	debug(NULL, "rant_change_string: Entered\n");

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	bu = unique_find(bot, &dl_mod_rant_unique);
	if (bu)
		dl_mod_rant = (dlist_t **) & bu->data;

	if (opt == MOD_RANT_CLEAR) {
		str = rant_op_clear(bot, dl_mod_rant);

		return NULL;
	} else if (opt == MOD_RANT_SIZE) {
		str = rant_op_size(bot, dl_mod_rant);
	} else if (opt == MOD_RANT_PUSH) {
		str = rant_op_push(bot, dl_mod_rant, string);
	} else if (opt == MOD_RANT_POP) {
		str = rant_op_pop(bot, dl_mod_rant);
	} else if (opt == MOD_RANT_LIST) {
		str = rant_op_list(bot, dl_mod_rant);
	}

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

char *rant_op_clear(bot_t * bot, dlist_t ** dl)
{

	if (!bot || !dl)
		return NULL;

	dlist_fini(dl, free);

	return NULL;
}

char *rant_op_size(bot_t * bot, dlist_t ** dl)
{
	char *str = NULL;

	if (!bot || !dl)
		return NULL;

	str = str_unite("%i", dlist_size(*dl));
	return str;
}

char *rant_op_push(bot_t * bot, dlist_t ** dl, char *string)
{
	unique_t *bu = NULL;

	debug(NULL, "rant_op_push: Entered: dl=%p\n", dl);

	if (!bot || !string)
		return NULL;

	if (!dl) {
		bu = unique_create(bot, &dl_mod_rant_unique,
				   UNIQUE_ID_TAG | UNIQUE_ID_NICK);
		if (!bu)
			return NULL;
		dl = (dlist_t **) & bu->data;
	}

	string = str_unite("%s\n", string);
	dlist_Dinsert_after(dl, string);

	return NULL;
}

char *rant_op_pop(bot_t * bot, dlist_t ** dl)
{
	unique_t *bu = NULL;
	char *str = NULL;

	debug(NULL, "rant_op_pop: Entered\n");

	if (!bot || !dl)
		return NULL;

	if (!*dl)
		return NULL;

	str = dlist_to_str(*dl);
	dlist_fini(dl, free);
	bu = unique_delete(bot, &dl_mod_rant_unique, NULL);
	free(bu);
	return str;
}

char *rant_op_list(bot_t * bot, dlist_t ** dl)
{
	dlist_t *dl_bu_list = NULL, *dptr_bu = NULL;
	unique_t *bu_tmp = NULL;
	char *str = NULL;

	dlist_fornext(dl_mod_rant_unique, dptr_bu) {
		bu_tmp = (unique_t *) dlist_data(dptr_bu);
		str = unique_bu2str(bu_tmp, UNIQUE_BU2STR_NEWLINE);
		dlist_Dinsert_after(&dl_bu_list, str);
		str = dlist_to_str(dl_bu_list);
		dlist_fini(&dl_bu_list, free);
	}

	return str;
}

bot_t *rant_input(dlist_t * dlist_node, bot_t * bot)
{
	unique_t *bu = NULL;
	dlist_t **dl = NULL;
	char *str = NULL;

	debug(NULL, "rant_input: Entered\n");

	bu = unique_find(bot, &dl_mod_rant_unique);
	if (!bu)
		return NULL;

	if (strcasestr(bot->txt_data_in, "^rant(pop)"))
		return NULL;

	dl = (dlist_t **) & bu->data;

	str = str_unite("%s\n", bot->txt_data_in);
	dlist_Dinsert_after(dl, str);

	return NULL;
}
