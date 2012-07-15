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
#include "mod_color.h"

void __color_init__(void)
{

	strlcpy_buf(mod_color_info.name, "mod_color");
	strlcpy_buf(mod_color_info.trigger, "^color");

	module_add_subtrigs(&mod_color_info, "^fgcolor");
	module_add_subtrigs(&mod_color_info, "^bgcolor");

	module_add_subtrigs(&mod_color_info, "^fgc");
	module_add_subtrigs(&mod_color_info, "^bgc");

	mod_color_info.init = color_init;
	mod_color_info.fini = color_fini;
	mod_color_info.help = color_help;
	mod_color_info.run = color_run;

	mod_color_info.output = NULL;
	mod_color_info.input = NULL;

	debug(NULL, "__color_init__: Loaded mod_color\n");

	return;
}

bot_t *color_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "color_init: Entered\n");
	return NULL;
}

bot_t *color_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "color_fini: Entered\n");
	return NULL;
}

bot_t *color_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "color_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^color || ^{fg|bg}color(white:black:blue:green:red:brown:purple:orange:yellow:lime:teal:cyan:royal:pink:grey:silver)";

	return NULL;
}

bot_t *color_run(dlist_t * dlist_node, bot_t * bot)
{

	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt, sub;
	char *color_str, *COLOR_STR = NULL;

	debug(bot, "color_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "color_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	dl_module_arg_after_options = bot->dl_module_arg;

	opt = sub = 0;
	color_str = NULL;

	if (str_match
	    (bot->trig_called, STR_MATCH_STRCASECMP, 0, "^fgcolor", "^fgc",
	     NULL)) {
		sub = MOD_COLOR_FGCOLOR;
		COLOR_STR = IRC_COLOR_COLOR_STR;
	} else
	    if (str_match
		(bot->trig_called, STR_MATCH_STRCASECMP, 0, "^bgcolor", "^bgc",
		 NULL)) {
		sub = MOD_COLOR_BGCOLOR;
		COLOR_STR = IRC_COLOR_REVERSE2_STR;
	}

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "white")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_WHITE_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "black")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_BLACK_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "blue")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_BLUE_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "green")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_GREEN_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "red")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_RED_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "brown")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_BROWN_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "purple")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_PURPLE_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "orange")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_ORANGE_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "yellow")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_YELLOW_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "lime")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_LIME_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "teal")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_TEAL_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "cyan")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_CYAN_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "royal")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_ROYAL_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "pink")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_PINK_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "grey")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_GREY_STR);
	} else if (!strncasecmp_len(dl_options_ptr, "silver")) {
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_SILVER_STR);
	} else
		color_str =
		    str_unite("%s%s%s", COLOR_STR,
			      sub ==
			      MOD_COLOR_BGCOLOR ? IRC_COLOR_COLOR_STR : "",
			      IRC_COLOR_PINK_STR);

	MOD_OPTIONS_BOTTOM_HALF;

	if (!color_str)
		return NULL;

	MOD_PARSE_TOP_HALF;
	l_new_str = color_change_string(l_str_ptr, color_str, sub);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *color_change_string(char *string, char *color_str, int sub)
{

	char *sep_ptr;
	char *str = NULL;

	if (!string || !color_str)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	str = str_unite("%s%s%s", color_str, string, IRC_COLOR_RESET_STR);

	return str;
}
