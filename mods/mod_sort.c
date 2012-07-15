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
#include "mod_sort.h"

void __sort_init__(void)
{

	strlcpy_buf(mod_sort_info.name, "mod_sort");
	strlcpy_buf(mod_sort_info.trigger, "^sort");

	module_add_subtrigs(&mod_sort_info, "^uniqc");
	module_add_subtrigs(&mod_sort_info, "^uniqw");
	module_add_subtrigs(&mod_sort_info, "^uniq");

	mod_sort_info.init = sort_init;
	mod_sort_info.fini = sort_fini;
	mod_sort_info.help = sort_help;
	mod_sort_info.run = sort_run;

	mod_sort_info.output = NULL;
	mod_sort_info.input = NULL;

	debug(NULL, "__sort_init__: Loaded mod_sort\n");

	return;
}

bot_t *sort_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "sort_init: Entered\n");
	return NULL;
}

bot_t *sort_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "sort_fini: Entered\n");

	return NULL;
}

bot_t *sort_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "sort_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^sort || ^sort(forw:backw:randw:forc:backc:randc:uniqc:uniqw:uniq) || ^sort(...,<optional_delim>)";

	return NULL;
}

bot_t *sort_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr, *opt_delim = NULL;
	int opt;

	debug(bot, "sort_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "sort_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = SORT_FORW;

	if (!strcasecmp(bot->trig_called, "^uniq")) {
		opt = SORT_UNIQW;
	} else if (!strcasecmp(bot->trig_called, "^uniqc")) {
		opt = SORT_UNIQC;
	} else if (!strcasecmp(bot->trig_called, "^uniqw")) {
		opt = SORT_UNIQW;
	}

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "forw"))
		opt = SORT_FORW;
	else if (!strncasecmp_len(dl_options_ptr, "backw"))
		opt = SORT_BACKW;
	else if (!strncasecmp_len(dl_options_ptr, "randw"))
		opt = SORT_RANDW;
	else if (!strncasecmp_len(dl_options_ptr, "forc"))
		opt = SORT_FORC;
	else if (!strncasecmp_len(dl_options_ptr, "backc"))
		opt = SORT_BACKC;
	else if (!strncasecmp_len(dl_options_ptr, "randc"))
		opt = SORT_RANDC;
	else if (!strncasecmp_len(dl_options_ptr, "uniqc")) {
		opt = SORT_UNIQC;
	} else if (!strncasecmp_len(dl_options_ptr, "uniqw")) {
		opt = SORT_UNIQW;
	}

	opt_delim = _strchr(dl_options_ptr, ',');
	if (opt_delim)
		opt_delim = opt_delim + 1;

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str =
	    sort_change_string(l_str_ptr, strlen(l_str_ptr), opt, opt_delim);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *sort_change_string(char *string, int string_size, int opt, char *delim)
{
	int i, word = 0;
	char *str = NULL;
	char **str_p_array;
	char *sep_ptr;
	str_p_array = NULL;

	if (!string)
		return 0;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	debug(NULL,
	      "sort_change_string: Entered: string=%s, string_size=%i, opt=%i\n",
	      string, string_size, opt);

	word = 0;
/* find words */

	if (!_sNULL(delim))
		delim = " ";

	if (opt == SORT_FORC || opt == SORT_BACKC || opt == SORT_RANDC
	    || opt == SORT_UNIQC) {
		delim = "";
		str_p_array =
		    (char **)calloc(strlen(string) + 1, sizeof(char *));
		if (!str_p_array)
			return NULL;

		for (i = 0; i < strlen(string); i++) {
			str_p_array[i] = strdup_char(string[i]);
			word++;
		}

	} else if (opt == SORT_FORW || opt == SORT_BACKW || opt == SORT_RANDW
		   || opt == SORT_UNIQW) {

		str_p_array =
		    tokenize_array(NULL, string,
				   TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE |
				   TOKENIZE_LEAVEQUOTES, delim, &word);
	}

	if (!str_p_array)
		return NULL;

	if (opt == SORT_UNIQC || opt == SORT_UNIQW) {
		tokenize_sort_strings(str_p_array, &word,
				      TOKENIZE_SORT_STRINGS_UNIQ);
	}

	if (opt == SORT_FORW || opt == SORT_FORC) {
		tokenize_sort_strings(str_p_array, &word,
				      TOKENIZE_SORT_STRINGS_FORWARD);
	} else if (opt == SORT_BACKW || opt == SORT_BACKC) {
		tokenize_sort_strings(str_p_array, &word,
				      TOKENIZE_SORT_STRINGS_BACKWARD);
	} else if (opt == SORT_RANDW || opt == SORT_RANDC) {
		tokenize_sort_strings(str_p_array, &word,
				      TOKENIZE_SORT_STRINGS_RANDWARD);
	}

	str = arraystr_to_str(str_p_array, word, delim);

	tokenize_destroy_array(NULL, str_p_array);

	return str;
}
