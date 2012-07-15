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
#include "mod_sep.h"

void __sep_init__(void)
{

	strlcpy_buf(mod_sep_info.name, "mod_sep");
	strlcpy_buf(mod_sep_info.trigger, "^sep");

	module_add_subtrigs(&mod_sep_info, "^:");
	module_add_subtrigs(&mod_sep_info, "^!");
	module_add_subtrigs(&mod_sep_info, "^#");

	mod_sep_info.init = sep_init;
	mod_sep_info.fini = sep_fini;
	mod_sep_info.help = sep_help;
	mod_sep_info.run = sep_run;

	mod_sep_info.output = NULL;
	mod_sep_info.input = NULL;

	debug(NULL, "__sep_init__: Loaded mod_sep\n");

	return;
}

bot_t *sep_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "sep_init: Entered\n");
	return NULL;
}

bot_t *sep_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "sep_fini: Entered\n");
	return NULL;
}

bot_t *sep_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "sep_help: Entered\n");

	if (!bot)
		return NULL;

	if (str_match(bot->trig_called, STR_MATCH_STRCASECMP, 0, "^:", NULL)) {
		bot->dl_module_help =
		    "^: ::: breaks up output text so that subsequent triggers may not modify text prior to a ^:";
	} else
	    if (str_match
		(bot->trig_called, STR_MATCH_STRCASECMP, 0, "^!", NULL)) {
		bot->dl_module_help =
		    "^! ::: crushes all breaks, allows full text to be modified once again";
	} else
	    if (str_match
		(bot->trig_called, STR_MATCH_STRCASECMP, 0, "^#", NULL)) {
		bot->dl_module_help =
		    "^# ::: initializes or ends a comment, text in between comment triggers is not processed";
}
	 else {
		bot->dl_module_help = "^sep ::: ^: || ^!";
	}

	return NULL;
}

bot_t *sep_run(dlist_t * dlist_node, bot_t * bot)
{
	int sub = 0;

	debug(bot, "sep_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "sep_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

/*
  NEED THIS DISABLED SO ^# will work after it's set
  if (_bot_shouldreturn (bot))
    return NULL;
*/

	if (!strcasecmp(bot->trig_called, "^:")) {
		sub = MOD_SEP_BREAK;
	} else if (!strcasecmp(bot->trig_called, "^!")) {
		sub = MOD_SEP_CRUSH;
	} else if (!strcasecmp(bot->trig_called, "^#")) {
		sub = MOD_SEP_COMMENT;
	}

	else
		return NULL;

	if (sub == MOD_SEP_BREAK) {
		charcat_bot(bot->txt_data_out, 0x01);
		strlcat_bot(bot->txt_data_out, bot->txt_data_in);
		strlcat_bot(bot->txt_data_out,
			    _eat_whitespace(bot->dl_module_arg));
	} else if (sub == MOD_SEP_CRUSH) {
		str_clean_sep_shrink(bot->txt_data_out,
				     strlen(bot->txt_data_out) + 1);
		strlcat_bot(bot->txt_data_out, bot->txt_data_in);
		strlcat_bot(bot->txt_data_out,
			    _eat_whitespace(bot->dl_module_arg));
	} else if (sub == MOD_SEP_COMMENT) {
		if (bot->iscomment)
			bot->iscomment = 0;
		else
			bot->iscomment = 1;
	}

	return bot;
}
