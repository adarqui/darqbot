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
#include "mod_hun.h"

void __hun_init__(void)
{

	strlcpy_buf(mod_hun_info.name, "mod_hun");
	strlcpy_buf(mod_hun_info.trigger, "^hun");

	mod_hun_info.init = hun_init;
	mod_hun_info.fini = hun_fini;
	mod_hun_info.help = hun_help;
	mod_hun_info.run = hun_run;

	mod_hun_info.output = NULL;
	mod_hun_info.input = NULL;

	debug(NULL, "__hun_init__: Loaded mod_hun\n");

	return;
}

bot_t *hun_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "hun_init: Entered\n");
	return NULL;
}

bot_t *hun_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "hun_fini: Entered\n");
	return NULL;
}

bot_t *hun_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "hun_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^hun || ^hun(en_US(english):en_CA(canadian):fr(french):ar(arabic)):de-DE(german):ro(romanian):vi(vietnamese)";

	return NULL;
}

bot_t *hun_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	char *opt_2 = NULL;

	debug(bot, "hun_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "hun_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	opt_2 = "en_US";

	MOD_OPTIONS_TOP_HALF;

	opt_2 = strtok(dl_options_ptr + 1, ")");
	if (!opt_2)
		return NULL;

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = hun_change_string(l_str_ptr, opt, opt_2);
	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *hun_change_string(char *string, int opt, char *opt_2)
{
/* opt_2 = language file */

	char *str = NULL;
	char buf[MAX_BUF_SZ];
	int i;
	char *sep_ptr;
	char *word;

	Hunhandle *hunh;
	int hunh_res;
	char **hunh_words;
	char hunh_path_1[132], hunh_path_2[132];

	if (!string || !opt_2)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	hun_clean_path(opt_2);

	bz(hunh_path_1);
	bz(hunh_path_2);

	strlcpy_buf(hunh_path_1,
		    str_unite_static("%s/%s.aff", MOD_HUNH_DIR, opt_2));
	if (!_sNULL(hunh_path_1))
		return NULL;

	strlcpy_buf(hunh_path_2,
		    str_unite_static("%s/%s.dic", MOD_HUNH_DIR, opt_2));
	if (!_sNULL(hunh_path_2))
		return NULL;

	hunh = Hunspell_create(hunh_path_1, hunh_path_2);
	if (!hunh)
		return NULL;

	word = string;
	while (1) {
		word = strtok(word, " ");
		if (!word)
			break;

		hunh_res = Hunspell_spell(hunh, word);
		if (hunh_res == 1) {
			strlcat_bot(buf, word);
			charcat_bot(buf, ' ');
		} else {
			hunh_res = Hunspell_suggest(hunh, &hunh_words, word);
			if (!hunh_res) {
				strlcat_bot(buf, word);
				charcat_bot(buf, ' ');
			} else {
				strlcat_bot(buf, word);
				strlcat_bot(buf, "={");
				for (i = 0; i < hunh_res; i++) {
					strlcat_bot(buf, hunh_words[i]);
					if (i + 1 < hunh_res)
						charcat_bot(buf, ',');
				}
				strlcat_bot(buf, "} ");

			}
		}

		word = NULL;
	}

	Hunspell_destroy(hunh);

	if (_sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

int hun_clean_path(char *str)
{
	if (!str)
		return 0;
	while (*str) {
		if (*str == '_') {
		} else if (!isalpha(*str)) {
			*str = '\0';
		}
		str++;
	}

	return 0;
}
