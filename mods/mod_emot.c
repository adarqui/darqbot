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
#include "mod_emot.h"

void __emot_init__(void)
{
	strlcpy_buf(mod_emot_info.name, "mod_emot");
	strlcpy_buf(mod_emot_info.trigger, "^emot");

	mod_emot_info.init = emot_init;
	mod_emot_info.fini = emot_fini;
	mod_emot_info.help = emot_help;
	mod_emot_info.run = emot_run;

	mod_emot_info.output = NULL;
	mod_emot_info.input = NULL;

	debug(NULL, "__emot_init__: Loaded mod_emot\n");

	memset(&emot_info, 0, sizeof(emot_info));

	return;
}

void emot_fill_toc(bot_t * bot)
{
	int len_read, line_cur;
	char *str, *str2;
	char buf[10024];

	if (emot_info.initialized)
		return;

	if (emot_info.fp)
		fclose(emot_info.fp);
	emot_info.fp = NULL;

	str = str_unite_static("%s/mods/mod_emot_files/emots.txt", gi->confdir);

	debug(bot, "emot_fill_toc: emot emots.txt located at: %s\n", str);
	emot_info.fp = fopen(str, "r");

	if (!emot_info.fp)
		return;

	debug(bot, "emot_fill_toc: emots.txt FOUND!\n");

	line_cur = 0;
/* first pass, figure out max lines */
	while (1) {
		if (fgets(buf, sizeof(buf), emot_info.fp) == NULL)
			break;
		line_cur++;
		len_read = strlen(buf);
	}

	emot_info.max_toc = line_cur + 1;

	emot_info.toc =
	    (emot_toc_t *) malloc(sizeof(emot_toc_t) * emot_info.max_toc);
	memset(emot_info.toc, 0, sizeof(emot_toc_t) * emot_info.max_toc);

	line_cur = 0;
/* second pass, fill toc */

	fseek(emot_info.fp, 0, SEEK_SET);
	while (1) {
		if (fgets(buf, sizeof(buf), emot_info.fp) == NULL)
			break;

		line_cur++;
		len_read = strlen(buf);
		str2 = strstr(buf, ":::::");
		if (!str2)
			continue;

		strstrip_nl(buf);

		emot_info.toc[line_cur].emot = strtokdup_str(buf, ":::::");
		if (!emot_info.toc[line_cur].emot)
			continue;

		emot_info.toc[line_cur].desc = strdup(str2 + strlen(":::::"));
	}

	emot_info.initialized = 1;

/* causing a ton of memory allocs */
	qsort(emot_info.toc, emot_info.max_toc, sizeof(emot_toc_t),
	      emot_rand_compare);

	return;
}

void emot_print_toc(bot_t * bot)
{
	int i;

	if (!emot_info.initialized) {
		debug(bot, "emot_print_toc: NOT INITIALIZED\n");
		return;
	}

	for (i = 0; i < emot_info.max_toc; i++) {
		debug(bot, "emot_print_toc: toc[%i].%s = %s\n", i,
		      emot_info.toc[i].emot, emot_info.toc[i].desc);
	}

	debug(bot, "emot_print_toc: max_toc=%i\n", emot_info.max_toc);

	return;
}

bot_t *emot_init(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "emot_init: Entered\n");

	if (!emot_info.initialized) {
		emot_fill_toc(bot);
	}

	return NULL;
}

bot_t *emot_fini(dlist_t * dlist_node, bot_t * bot)
{

	int i;

	debug(bot, "emot_fini: Entered\n");

	if (emot_info.fp)
		fclose(emot_info.fp);
	emot_info.fp = NULL;

	emot_info.initialized = 0;
	for (i = 0; i < emot_info.max_toc; i++) {
		if (emot_info.toc[i].emot)
			free(emot_info.toc[i].emot);
		if (emot_info.toc[i].desc)
			free(emot_info.toc[i].desc);
	}

	memset(emot_info.toc, 0, emot_info.max_toc * sizeof(emot_toc_t));

	if (emot_info.toc)
		free(emot_info.toc);

	memset(&emot_info, 0, sizeof(emot_info));

	return NULL;
}

bot_t *emot_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "emot_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^emot || ^emot <keyword>";

	return NULL;

}

bot_t *emot_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt_1;
	char *opt_3;

	debug(bot, "emot_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "emot_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt_1 = 0;
	opt_3 = NULL;

	if (!emot_info.max_toc)
		return bot;

	MOD_OPTIONS_TOP_HALF;
	if (strstr(bot->dl_module_arg + 1, "print")) {
		opt_3 = "print";
		emot_print_toc(bot);
		return bot;
	} else if (strstr(bot->dl_module_arg + 1, "shuffle")) {
		opt_3 = "shufle";

		qsort(emot_info.toc, emot_info.max_toc, sizeof(emot_toc_t),
		      emot_rand_compare);

		return (bot);
	}
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF;
	l_new_str = emot_change_string(l_str_ptr, opt_1, opt_3);
	MOD_PARSE_BOTTOM_HALF;

	if (opt_3)
		free(opt_3);

	return bot;
}

char *emot_get(char *string)
{
	int i, rand_val;

	if (!string)
		return NULL;

	if (strlen(string) < 3)
		return NULL;

	rand_val = rand() % (emot_info.max_toc + 1);

/* forward */
	for (i = rand_val; i < emot_info.max_toc; i++) {

		if (!emot_info.toc[i].desc)
			continue;

		if (xstrcasestr_unique(emot_info.toc[i].desc, string)) {
			return strdup(emot_info.toc[i].emot);
		}
	}

/* backward */
	for (i = rand_val; i >= 0; i--) {

		if (!emot_info.toc[i].desc)
			continue;

		if (xstrcasestr_unique(emot_info.toc[i].desc, string)) {
			return strdup(emot_info.toc[i].emot);
		}
	}

	return NULL;
}

char *emot_change_string(char *string, int opt_1, char *opt_3)
{
	int d;
	char *str, *syn, *tok_ptr;
	char buf[MAX_BUF_SZ];

	str = NULL;

	char *sep_ptr;

/* opt_1 = minimum 
   opt_2 = maximum
   opt_3 = grep string
 */

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	if (opt_1 == 1) {
	}

	d = 0;
	tok_ptr = string;
	while (1) {
		tok_ptr = strtok(tok_ptr, " ");
		if (!tok_ptr)
			break;

		syn = emot_get(tok_ptr);
		if (syn) {
			if (d)
				charcat_bot(buf, ' ');
			strlcat_bot(buf, syn);
			d++;
		} else {
			if (d)
				charcat_bot(buf, ' ');
			strlcat_bot(buf, tok_ptr);
			d++;
		}

		tok_ptr = NULL;
	}

	if (buf[0] != 0)
		str = strdup(buf);

	return str;
}

int emot_rand_compare(const void *v1, const void *v2)
{
	int vals[3], r;

	vals[0] = -1;
	vals[1] = 0;
	vals[2] = 1;

	r = rand() % 4;

	return vals[r];
}
