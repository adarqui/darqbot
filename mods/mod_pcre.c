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
#include "mod_pcre.h"

void __pcre_init__(void)
{
	strlcpy_buf(mod_pcre_info.name, "mod_pcre");
	strlcpy_buf(mod_pcre_info.trigger, "^pcre");

	mod_pcre_info.init = pcre_init;
	mod_pcre_info.fini = pcre_fini;
	mod_pcre_info.help = pcre_help;
	mod_pcre_info.run = pcre_run;

	mod_pcre_info.output = NULL;
	mod_pcre_info.input = NULL;

	debug(NULL, "__pcre_init__: Loaded mod_pcre\n");

	return;
}

bot_t *pcre_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "pcre_init: Entered\n");
	return NULL;
}

bot_t *pcre_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "pcre_fini: Entered\n");
	return NULL;
}

bot_t *pcre_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "pcre_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^pcre /<text>/ || ^pcre(-i -v ~v -u -e -V) /<text>/";

	return NULL;
}

bot_t *pcre_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "pcre_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "pcre_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;

	if (xstrcasestr_len(dl_options_ptr, "-i", len_options_area)) {
		opt |= OPT_NOCASE;
	}
	if (xstrcasestr_len(dl_options_ptr, "-e", len_options_area)) {
		opt |= OPT_EXTENDED;
	}
	if (strstr(dl_options_ptr, "-V")) {
		opt &= ~OPT_EXTRACT;
		opt |= OPT_REMOVE;
	}
	if (strstr(dl_options_ptr, "-v")) {
		opt &= ~OPT_EXTRACT;
		opt |= OPT_REMOVELINE;
	}
	if (xstrcasestr_len(dl_options_ptr, "~v", len_options_area)) {
		opt &= ~OPT_REMOVE;
		opt |= OPT_EXTRACT;
	}
	if (xstrcasestr_len(dl_options_ptr, "-u", len_options_area)) {
		opt |= OPT_UNGREEDY;
	}

	MOD_OPTIONS_BOTTOM_HALF;

/* verbose checks */
/*
  if (opt & OPT_NOCASE)
    {
      puts ("OPT_NOCASE");
    }
  if (opt & OPT_EXTENDED)
    {
      puts ("OPT_EXTENDED");
    }
  if (opt & OPT_REMOVE)
    {
      puts ("OPT_REMOVE");
    }
  if (opt & OPT_EXTRACT)
    {
      puts ("OPT_EXTRACT");
    }
  if (opt & OPT_UNGREEDY)
    {
      puts ("OPT_UNGREEDY");
    }
  if (opt & OPT_REMOVELINE)
    {
      puts ("OPT_REMOVELINE");
    }
*/

	MOD_PARSE_TOP_HALF_ARG;
	l_new_str = pcre_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_ARG;

	return bot;
}

char *pcre_change_string(bot_t * bot, char *string, int opt)
{

	char *txt_ptr;
	char *str = NULL, *t_ptr, *t_ptr_2;
	char buf[MAX_BUF_SZ], lbuf[MAX_BUF_SZ];
	int i, j;

	char *sep_ptr;

	const char *errstr_pcre;
	int erroffset_pcre;
	pcre *expr_pcre;

	int len_pcre;
	int offset_pcre;
	int flags_pcre;
	int options_pcre;

	int matches_pcre[2];

	len_pcre = offset_pcre = flags_pcre = options_pcre = 0;

	if (!string || !bot) {
		return NULL;
	}

	debug(bot, "pcre_change_string: Entered: %s\n", bot->txt_data_out);

	buf[0] = '\0';
	_memset(lbuf, 0, sizeof(lbuf));

	txt_ptr = bot->txt_data_out;
	sep_ptr = str_find_sep(txt_ptr);
	if (sep_ptr)
		txt_ptr = sep_ptr;

	string = _strchr(string, '/');
	if (!string)
		return NULL;
	string++;

	t_ptr = _strrchr(string, '/');
	if (!t_ptr)
		return NULL;

	t_ptr_2 = _eat_whitespace(t_ptr + 1);
	if (t_ptr_2 && strlen(t_ptr_2)) {
		strlcat_bot(txt_ptr, t_ptr_2);
	}
	*t_ptr = '\0';

	if (opt & OPT_EXTENDED)
		options_pcre |= PCRE_EXTENDED;

	if (opt & OPT_NOCASE)
		options_pcre |= PCRE_CASELESS;

	if (opt & OPT_UNGREEDY)
		options_pcre |= PCRE_UNGREEDY;

	expr_pcre =
	    pcre_compile(string, options_pcre, &errstr_pcre, &erroffset_pcre,
			 0);
	if (!expr_pcre)
		return NULL;

	len_pcre = strlen(txt_ptr);
	offset_pcre = 0;
	flags_pcre = 0;

	bz2(matches_pcre);

	j = 0;
	while (1) {
		if (j == 500)
			break;

		i = pcre_exec(expr_pcre, NULL, txt_ptr, len_pcre, offset_pcre,
			      flags_pcre, matches_pcre, 2);
		if (i <= 0)
			break;

		bz(buf);

		if ((matches_pcre[1] - matches_pcre[0])
		    && (opt & OPT_REMOVELINE)) {
			strzero_bot(bot->txt_data_out);
			return NULL;
		}

		if (opt & OPT_EXTRACT) {
			snprintf_buf(buf, "%.*s",
				     matches_pcre[1] - matches_pcre[0],
				     txt_ptr + matches_pcre[0]);
			printf("EXTRACTING [%s], %i\n", buf,
			       matches_pcre[1] - matches_pcre[0]);
		} else if (opt & OPT_REMOVE) {
			snprintf_buf(buf, "%.*s", matches_pcre[0] - offset_pcre,
				     txt_ptr + offset_pcre);
			printf("REMOVING [%s], %i\n", buf,
			       matches_pcre[0] - offset_pcre);
		} else {
			snprintf_buf(buf, "%.*s%s%.*s%s",
				     matches_pcre[0] - offset_pcre,
				     txt_ptr + offset_pcre, IRC_COLOR_BOLD_STR,
				     matches_pcre[1] - matches_pcre[0],
				     txt_ptr + matches_pcre[0],
				     IRC_COLOR_RESET_STR);
		}

		strlcat_bot(lbuf, buf);

		offset_pcre = matches_pcre[1];
		flags_pcre |= PCRE_NOTBOL;

		j++;
	}

	if (_sNULL(buf) != NULL) {
		if (!(opt & OPT_EXTRACT))
			strlcat_bot(lbuf, bot->txt_data_out + offset_pcre);
	}

	if ((opt & OPT_REMOVE) && _sNULL(buf) == NULL) {
		strlcat_bot(lbuf, bot->txt_data_out + offset_pcre);
	}

	if (_sNULL(lbuf) != NULL)
		str = strdup(lbuf);

	return str;
}
