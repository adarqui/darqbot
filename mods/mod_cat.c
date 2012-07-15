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
#include "mod_cat.h"

void __cat_init__(void)
{

	strlcpy_buf(mod_cat_info.name, "mod_cat");
	strlcpy_buf(mod_cat_info.trigger, "^cat");

	mod_cat_info.init = cat_init;
	mod_cat_info.fini = cat_fini;
	mod_cat_info.help = cat_help;
	mod_cat_info.run = cat_run;

	mod_cat_info.output = NULL;
	mod_cat_info.input = NULL;

	debug(NULL, "__cat_init__: Loaded mod_cat\n");

	return;
}

bot_t *cat_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "cat_init: Entered\n");
	return NULL;
}

bot_t *cat_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "cat_fini: Entered\n");
	return NULL;
}

bot_t *cat_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "cat_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^cat";

	return NULL;
}

bot_t *cat_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "cat_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "cat_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = MOD_CAT_OPT_READ;

	MOD_OPTIONS_TOP_HALF;

	dl_options_ptr = _eat_whitespace(dl_options_ptr);

	if (!strcasecmp(dl_options_ptr, "<")) {
		opt = MOD_CAT_OPT_READ;
	} else if (!strcasecmp(dl_options_ptr, ">")) {
		opt = MOD_CAT_OPT_WRITE;
	} else if (!strcasecmp(dl_options_ptr, ">>")) {
		opt = MOD_CAT_OPT_APPEND;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = cat_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *cat_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;

	char *sep_ptr;

	debug(NULL, "cat_change_string: Entered\n");

	if (!string || !bot)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	if (opt == MOD_CAT_OPT_READ) {
		str = cat_op_read(string);
	} else {
		str = cat_op_write(bot, string, opt);
	}

	return str;
}

char *cat_op_write(bot_t * bot, char *files, int opt)
{
	dlist_t *dl_files = NULL, *dptr;
	FILE *fp = NULL;
	char *str = NULL, *open_opt = NULL, *fname = NULL;

	debug(NULL, "cat_op_write: Entered\n");

	if (!bot || !files)
		return NULL;

	if (opt == MOD_CAT_OPT_WRITE)
		open_opt = "w";
	else if (opt == MOD_CAT_OPT_APPEND)
		open_opt = "a";
	else
		return NULL;

	dl_files =
	    tokenize(NULL, files, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE,
		     " ");
	if (!dl_files)
		return NULL;

	dlist_fornext(dl_files, dptr) {
		fname = (char *)dlist_data(dptr);
		if (!fname)
			continue;

		if (cat_illegal_fname(fname))
			continue;

		fname =
		    str_unite_static("%s/mods/mod_cat_files/%s", gi->confdir,
				     fname);
		debug(NULL, "cat_op_write: fname=%s\n", fname);
		fp = fopen(fname, open_opt);
		if (!fp)
			continue;

		fwrite(bot->txt_data_out, 1, strlen(bot->txt_data_out), fp);
		fflush(fp);
		fclose(fp);
	}

	tokenize_destroy(NULL, &dl_files);

	strzero_bot(bot->txt_data_out);

	return str;
}

char *cat_op_read(char *files)
{
	dlist_t *dl_text = NULL, *dl_files = NULL, *dptr = NULL;
	FILE *fp = NULL;
	char *str = NULL, buf[1024], *buf_str, *fname;

	debug(NULL, "cat_op_read: Entered\n");

	if (!files)
		return NULL;

	dl_files =
	    tokenize(NULL, files, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE,
		     " ");
	if (!dl_files)
		return NULL;

	dlist_fornext(dl_files, dptr) {
		fname = (char *)dlist_data(dptr);
		if (cat_illegal_fname(fname))
			continue;

		fname =
		    str_unite_static("%s/mods/mod_cat_files/%s", gi->confdir,
				     fname);
		fp = fopen(fname, "r");
		if (!fp)
			continue;

		debug(NULL, "cat_op_read: fname=%s\n", fname);

		while (1) {
			_memset(buf, 0, sizeof(buf));
			if (fgets(buf, sizeof(buf) - 1, fp) == NULL)
				break;
			buf_str = strdup(buf);
			dlist_Dinsert_after(&dl_text, buf_str);
		}

		fclose(fp);
	}

	str = dlist_to_str(dl_text);
	dlist_fini(&dl_text, free);
	tokenize_destroy(NULL, &dl_files);

	return str;
}

int cat_illegal_fname(char *f)
{
	char *s;

	if (!f)
		return 1;

	s = _strchr(f, '/');
	if (s)
		return 1;

	return 0;
}
