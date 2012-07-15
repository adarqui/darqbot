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
#include "mod_help.h"

void __help_init__(void)
{

	strlcpy_buf(mod_help_info.name, "mod_help");
	strlcpy_buf(mod_help_info.trigger, "^help");

	mod_help_info.init = help_init;
	mod_help_info.fini = help_fini;
	mod_help_info.help = help_help;
	mod_help_info.run = help_run;

	mod_help_info.output = NULL;
	mod_help_info.input = NULL;

	debug(NULL, "__help_init__: Loaded mod_help\n");

	return;
}

bot_t *help_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "help_init: Entered\n");
	return NULL;
}

bot_t *help_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "help_fini: Entered\n");
	return NULL;
}

bot_t *help_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "help_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^help || ^help ^trigger";

	return bot;
}

bot_t *help_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "help_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "help_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = help_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *help_change_string(bot_t * bot, char *string, int opt)
{
	dlist_t *dl_text = NULL;
	char *str = NULL, *tok;
	char *sep_ptr;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	tok = strtok(string, " ");

	if (!bot || !string)
		return NULL;

	if (_sNULL(tok)) {
		help_get(bot, &dl_text, tok);
	} else {
		dlist_traverse(&gi->dl_module, help_traverse, &dl_text);
	}

	str = dlist_to_str(dl_text);
	dl_str_destroy(&dl_text);

	return str;
}

void *help_traverse(void *pa, void *pb)
{
	dlist_t **dl_text = (dlist_t **) pa, *dptr_mod = (dlist_t *) pb;
	module_t *mod = NULL, *mod_root = NULL;
	int cntval = 0;

	if (!pa || !pb)
		return NULL;

	mod = (module_t *) dlist_data(dptr_mod);

	if (!mod)
		return NULL;
	if (mod->trigger == NULL)
		return NULL;

	printf("mod->trigger=%s\n", mod->trigger);

	mod_root = mod;

	if (mod->type & XMODULE_TYPE_MIRROR) {
		mod_root = mod->self;
		printf("mod_root->trigger=%s\n", mod_root->trigger);
	}

	cntval = stat_retcnt(NULL, mod_root->trigger);

	dl_str_unite(dl_text, "%s (%i),", mod->trigger, cntval);

	return NULL;
}

void help_get(bot_t * bot, dlist_t ** dl_text, char *trigger)
{
	module_t *mod = NULL;
	char trigger_orig[32];

	if (!bot || !dl_text || !_sNULL(trigger))
		return;

	mod = xmodule_find_by_trig(XMODULE_TYPE_MODULE, trigger);
	if (!mod)
		return;

	strlcpy_buf(trigger_orig, bot->trig_called);
	strlcpy_buf(bot->trig_called, trigger);

	mod->help(NULL, bot);
	if (bot->dl_module_help) {
		dl_str_unite(dl_text, "%s", bot->dl_module_help);
	}

	strlcpy_buf(bot->trig_called, trigger_orig);

	return;
}
