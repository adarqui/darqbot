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
#include "mod_char.h"

void __char_init__(void)
{

	strlcpy_buf(mod_char_info.name, "mod_char");
	strlcpy_buf(mod_char_info.trigger, "^char");

	mod_char_info.init = char_init;
	mod_char_info.fini = char_fini;
	mod_char_info.help = char_help;
	mod_char_info.run = char_run;

	mod_char_info.output = NULL;
	mod_char_info.input = NULL;

	debug(NULL, "__char_init__: Loaded mod_char\n");

	return;
}

bot_t *char_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "char_init: Entered\n");
	return NULL;
}

bot_t *char_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "char_fini: Entered\n");
	return NULL;
}

bot_t *char_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "char_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^char || ^char(0-5)";

	return NULL;
}

bot_t *char_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr, *tok_1, *tok_2;
	int opt_rand, opt_match;

	debug(bot, "char_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "char_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt_rand = RANDCHAR_SAME;
	opt_match = MATCHCHAR_SAME;

	MOD_OPTIONS_TOP_HALF;
	opt_rand = randchar_str(dl_options_ptr);

	tok_1 = strtok(dl_options_ptr, ",");
	if (tok_1) {
		tok_2 = strtok(NULL, ")");
		if (tok_2) {
			opt_match = matchchar_str(tok_2);
		}
	}
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = char_change_string(l_str_ptr, opt_rand, opt_match);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *char_change_string(char *string, int opt_rand, int opt_match)
{
	int i, c;

	char *sep_ptr;

	if (!string)
		return NULL;

	debug(NULL, "char_change_string: Enered\n");

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	randchar_seed(rand());

	for (i = 0; i < strlen(string); i++) {
		if (string[i] == '\n')
			continue;

		c = string[i];

		if (matchchar(opt_match, string[i])) {
			if (opt_match == MATCHCHAR_INVERT) {
				if (string[i] == ' ' || string[i] == '\t') {
					c = randchar(opt_rand, string[i]);
				} else {
					c = ' ';
				}
			} else {
				c = randchar(opt_rand, string[i]);
			}
		}

		string[i] = c;
	}

	return strdup(string);
}
