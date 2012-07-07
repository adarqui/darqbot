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
#include "mod_quote.h"

void __quote_init__(void)
{

	strlcpy_buf(mod_quote_info.name, "mod_quote");
	strlcpy_buf(mod_quote_info.trigger, "^quote");

	mod_quote_info.init = quote_init;
	mod_quote_info.fini = quote_fini;
	mod_quote_info.help = quote_help;
	mod_quote_info.run = quote_run;

	mod_quote_info.output = NULL;
	mod_quote_info.input = NULL;

	debug(NULL, "__quote_init__: Loaded mod_quote\n");

	return;
}

bot_t *quote_init(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "quote_init: Entered\n");

	quote_add_files_to_db();

	return NULL;
}

bot_t *quote_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "quote_fini: Entered\n");

	return NULL;
}

bot_t *quote_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "quote_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^quote || ^quote(?.txt)";

	return NULL;
}

bot_t *quote_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt_1;
	char *opt_title = NULL;

	debug(bot, "quote_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "quote_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt_1 = 0;

	opt_title = NULL;

	MOD_OPTIONS_TOP_HALF;
	opt_title = strdup(dl_options_ptr);
	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = quote_change_string(l_str_ptr, opt_title);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *quote_change_string(char *string, char *opt_title)
{
	dlist_t *dptr_pair = NULL;
	xdb_pair_t *pair = NULL;
	char *str;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	str = NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	if (!opt_title) {
		dptr_pair = (dlist_t *) dlist_node_rand(dl_quote_pairs);
		if (!dptr_pair)
			return NULL;
		pair = (xdb_pair_t *) dlist_data(dptr_pair);
	} else {
		pair = xdb_pair_find_by_key(dl_quote_pairs, opt_title);
	}

	if (!pair)
		return NULL;

	str = quote_get(pair, string);

	return str;
}

char *quote_get(xdb_pair_t * pair, char *string)
{
	DB *db = NULL;
	int count = 0, recnum = 0;
	char *str = NULL;
	xdb_pair_t *pair_2 = NULL;

	if (!pair)
		return NULL;

	db = xdb_open(pair->value);
	if (!db)
		return NULL;

	db->stat_print(db, 0);

	count = xdb_count(db);

	printf("count=%i\n", count);

	recnum = rand() % count;

	pair_2 = xdb_get_recnum(db, recnum);

	str = strdup(pair_2->value);

	if (db)
		xdb_fini(db);

	if (pair_2)
		xdb_pair_destroy(pair_2);

	return str;
}

void quote_add_files_to_db(void)
{
	xdb_pair_t *pair;
	dlist_t *dl, *dptr, *dptr_tmp;
	char *dir;
	char *suffix, *str, *str_tmp;

	debug(NULL, "quote_add_files_to_db: Entered\n");

	dir = str_unite_static("%s/mods/mod_quote_files/", gi->confdir);

	dl = files_get_listing(NULL, dir,
			       FILES_NORMAL | FILES_RECURSE | FILES_NODOT);
	if (!dl)
		goto cleanup;

	dlist_fornext_retarded(dl, dptr, dptr_tmp) {
		if (!dptr)
			break;

		str = (char *)dlist_data(dptr);
		if (!str)
			continue;

		suffix = strrchr(str, '.');

		if (suffix) {
			if (!strcasecmp(suffix, ".quot")) {
				dlist_remove_and_free(&dl, &dptr, free);
				continue;
			}
		}

		suffix = strrchr(str, '/');
		if (!suffix)
			continue;
		suffix++;

		pair =
		    xdb_pair_create(suffix, strlen(suffix), str, strlen(str));

		if (!pair)
			continue;

		capsup(pair->key);
		str_tmp = pair->value;
		pair->value = str_unite("%s.quot", pair->value);
		free(str_tmp);
		dlist_Dinsert_after(&dl_quote_pairs, pair);
	}

	dlist_fornext(dl, dptr) {
		debug(NULL, "quote_add_files_to_db: file=%s\n",
		      (char *)dlist_data(dptr));
		quote_add_files_to_db_file((char *)dlist_data(dptr));
	}

 cleanup:
	if (dl)
		files_destroy(NULL, &dl);

	return;
}

void quote_add_files_to_db_file(char *name)
{
	DB *db = NULL;
	FILE *fp;
	char *name_db;
	char buf[1024];

	char *tok_1 = NULL, *tok_2 = NULL;
	int count = 0;

	if (!name)
		return;

	name_db = strrchr(name, '/');
	if (!name_db)
		return;

	fp = fopen(name, "r");
	if (!fp)
		return;

	name_db = str_unite_static("%s.quot", name);
	unlink(name_db);
	db = xdb_open(name_db);

	if (!db)
		goto cleanup;

	while (1) {
		memset(buf, 0, sizeof(buf));

		if (fgets(buf, sizeof(buf) - 1, fp) == NULL)
			break;

		strstrip_nl(buf);

		tok_2 = eat_whitespace(buf);
		if (!sNULL(tok_2))
			continue;

		str_shrink_quotes(tok_2);
		if (!sNULL(tok_2))
			continue;

		if (buf[0] == '#')
			continue;

		if (!sNULL(buf))
			continue;

		tok_1 = itocstr(count);

		if (!sNULL(tok_1))
			continue;

		xdb_write(db, tok_1, tok_2);

		count++;
	}

 cleanup:
	if (fp)
		fclose(fp);

	if (db)
		xdb_fini(db);

	return;
}
