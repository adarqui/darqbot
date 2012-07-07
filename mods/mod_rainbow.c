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
#include "mod_rainbow.h"

void __rainbow_init__(void)
{

	strlcpy_buf(mod_rainbow_info.name, "mod_rainbow");
	strlcpy_buf(mod_rainbow_info.trigger, "^rainbow");

	mod_rainbow_info.init = rainbow_init;
	mod_rainbow_info.fini = rainbow_fini;
	mod_rainbow_info.help = rainbow_help;
	mod_rainbow_info.run = rainbow_run;

	mod_rainbow_info.output = NULL;
	mod_rainbow_info.input = NULL;

	debug(NULL, "__rainbow_init__: Loaded mod_rainbow\n");

	return;
}

bot_t *rainbow_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rainbow_init: Entered\n");
	return NULL;
}

bot_t *rainbow_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rainbow_fini: Entered\n");
	return NULL;
}

bot_t *rainbow_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "rainbow_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^rainbow";

	return NULL;
}

bot_t *rainbow_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dl;
	rainbow_colors_t rc[RC_MAX + 1];
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt, rc_index;

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot, "rainbow_run: Entered\n");

	debug(bot,
	      "rainbow_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = rc_index = 0;
	memset(rc, 0, sizeof(rc));

	MOD_OPTIONS_TOP_HALF;

	if (xstrcasestr_len(dl_options_ptr, "rand", len_options_area)) {
		opt |= RAINBOW_OPT_RAND;
	}

	if (xstrcasestr_len(dl_options_ptr, "nochar", len_options_area)) {
		opt |= RAINBOW_OPT_NOCHAR;
	}

	if (xstrcasestr_len(dl_options_ptr, "nospace", len_options_area)) {
		opt |= RAINBOW_OPT_NOSPACE;
	}

	if (xstrcasestr_len(dl_options_ptr, "fgdef", len_options_area)) {
		rainbow_defaults(rc, &rc_index, IRC_COLOR_COLOR_STR);
	}

	if (xstrcasestr_len(dl_options_ptr, "bgdef", len_options_area)) {
		rainbow_defaults(rc, &rc_index, IRC_COLOR_REVERSE2_STR);
	}

	dl = tokenize(bot, dl_options_ptr, TOKENIZE_NORMAL | TOKENIZE_NOQUOTES,
		      ",");
	if (!dl) {
		rainbow_defaults(rc, &rc_index, IRC_COLOR_COLOR_STR);
	} else {
		rainbow_add_colors(dl, rc, &rc_index, "test");
		tokenize_destroy(bot, &dl);
	}

	MOD_OPTIONS_BOTTOM_HALF;

	if (rc[0].val == NULL) {
		rainbow_defaults(rc, &rc_index, IRC_COLOR_COLOR_STR);
	} else {
		printf("RC VAL ISNT NULL, %s\n", rc[0].val);
	}

	MOD_PARSE_TOP_HALF;
	l_new_str = rainbow_change_string(l_str_ptr, opt, rc, rc_index);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *rainbow_change_string(char *string, int opt, rainbow_colors_t * rc,
			    int rc_index)
{
	int start_color;
	int i;
	char buf[MAX_BUF_SZ + 1];

	char *sep_ptr;
	char *str = NULL;

	if (!string || !rc || rc_index <= 0)
		return 0;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	i = 0;
	start_color = 0;
	while (*string) {

		if (opt & RAINBOW_OPT_RAND) {
			i = rand() % (rc_index + 1);
		}

		if (i > rc_index)
			i = start_color;

		if (*string == '\n') {
			start_color++;
			if (start_color > rc_index)
				start_color = 0;
			i = start_color;
		}

		if (rc[i].val == NULL)
			i = 0;

		if (opt & RAINBOW_OPT_NOSPACE && *string == ' ')
			charcat_safe(buf, ' ', sizeof(buf) - 1);
		else {

			strlcat_bot(buf, rc[i].type);

			if (!strcmp(rc[i].type, IRC_COLOR_REVERSE2_STR)) {
				strlcat_bot(buf, IRC_COLOR_COLOR_STR);
			}

			strlcat_bot(buf, rc[i].val);

			if (!(opt & RAINBOW_OPT_NOCHAR))
				charcat_bot(buf, *string);
			else {
				if (*string == '\r' || *string == '\n')
					charcat_bot(buf, *string);
				else
					charcat_bot(buf, ' ');
			}

			if (!strcmp(rc[i].type, IRC_COLOR_REVERSE2_STR)) {
				strlcat_bot(buf, IRC_COLOR_RESET_STR);
			}
		}

		string++;
		i++;
	}

	if (buf[0] != '0') {
		strlcat_bot(buf, IRC_COLOR_RESET_STR);
		str = strdup(buf);
	}

	return str;
}

int
rainbow_defaults(rainbow_colors_t * rc, int *rc_index, char *IRC_COLOR_TYPE_STR)
{
	if (!rc || !rc_index || !IRC_COLOR_TYPE_STR)
		return 0;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_WHITE_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_GREEN_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_RED_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_PURPLE_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_ORANGE_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_YELLOW_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

/*
rc[*rc_index].val = IRC_COLOR_GREY_STR;
rc[*rc_index].type = IRC_COLOR_TYPE_STR;
*rc_index = *rc_index + 1;

if(*rc_index >= RC_MAX)
return 0;
*/

	rc[*rc_index].val = IRC_COLOR_LIME_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_TEAL_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_CYAN_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_PINK_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	if (*rc_index >= RC_MAX)
		return 0;

	rc[*rc_index].val = IRC_COLOR_SILVER_STR;
	rc[*rc_index].type = IRC_COLOR_TYPE_STR;
	*rc_index = *rc_index + 1;

	printf("2=%i", *rc_index);

	return 0;
}

int
rainbow_add_colors(dlist_t * dl, rainbow_colors_t * rc, int *rc_index,
		   char *type)
{
	dlist_t *dptr;
	char *str;

	if (!dl || !rc || !rc_index || !type)
		return 0;

	if (*rc_index >= RC_MAX)
		return 0;

	if (rc[*rc_index].val) {
		*rc_index = *rc_index + 1;
	}

	dlist_fornext(dl, dptr) {
		str = (char *)dlist_data(dptr);
		if (!str)
			continue;

		if (*rc_index >= RC_MAX)
			break;

		if (!strcasecmp(str, "fgwhite")) {
			rc[*rc_index].val = IRC_COLOR_WHITE_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgblack")) {
			rc[*rc_index].val = IRC_COLOR_BLACK_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgblue")) {
			rc[*rc_index].val = IRC_COLOR_BLUE_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fggreen")) {
			rc[*rc_index].val = IRC_COLOR_GREEN_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgred")) {
			rc[*rc_index].val = IRC_COLOR_RED_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgbrown")) {
			rc[*rc_index].val = IRC_COLOR_BROWN_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgpurple")) {
			rc[*rc_index].val = IRC_COLOR_PURPLE_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgorange")) {
			rc[*rc_index].val = IRC_COLOR_ORANGE_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgyellow")) {
			rc[*rc_index].val = IRC_COLOR_YELLOW_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fglime")) {
			rc[*rc_index].val = IRC_COLOR_LIME_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgteal")) {
			rc[*rc_index].val = IRC_COLOR_TEAL_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgcyan")) {
			rc[*rc_index].val = IRC_COLOR_CYAN_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgroyal")) {
			rc[*rc_index].val = IRC_COLOR_ROYAL_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgpink")) {
			rc[*rc_index].val = IRC_COLOR_PINK_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fggrey")) {
			rc[*rc_index].val = IRC_COLOR_GREY_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "fgsilver")) {
			rc[*rc_index].val = IRC_COLOR_SILVER_STR;
			rc[*rc_index].type = IRC_COLOR_COLOR_STR;
			*rc_index = *rc_index + 1;
		}

		else if (!strcasecmp(str, "bgwhite")) {
			rc[*rc_index].val = IRC_COLOR_WHITE_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgblack")) {
			rc[*rc_index].val = IRC_COLOR_BLACK_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgblue")) {
			rc[*rc_index].val = IRC_COLOR_BLUE_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bggreen")) {
			rc[*rc_index].val = IRC_COLOR_GREEN_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgred")) {
			rc[*rc_index].val = IRC_COLOR_RED_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgbrown")) {
			rc[*rc_index].val = IRC_COLOR_BROWN_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgpurple")) {
			rc[*rc_index].val = IRC_COLOR_PURPLE_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgorange")) {
			rc[*rc_index].val = IRC_COLOR_ORANGE_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgyellow")) {
			rc[*rc_index].val = IRC_COLOR_YELLOW_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bglime")) {
			rc[*rc_index].val = IRC_COLOR_LIME_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgteal")) {
			rc[*rc_index].val = IRC_COLOR_TEAL_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgcyan")) {
			rc[*rc_index].val = IRC_COLOR_CYAN_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgroyal")) {
			rc[*rc_index].val = IRC_COLOR_ROYAL_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgpink")) {
			rc[*rc_index].val = IRC_COLOR_PINK_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bggrey")) {
			rc[*rc_index].val = IRC_COLOR_GREY_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		} else if (!strcasecmp(str, "bgsilver")) {
			rc[*rc_index].val = IRC_COLOR_SILVER_STR;
			rc[*rc_index].type = IRC_COLOR_REVERSE2_STR;
			*rc_index = *rc_index + 1;
		}

	}

	return 0;
}
