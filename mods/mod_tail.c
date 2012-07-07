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
#include "mod_tail.h"

void __tail_init__(void)
{

	strlcpy_buf(mod_tail_info.name, "mod_tail");
	strlcpy_buf(mod_tail_info.trigger, "^tail");

	mod_tail_info.init = tail_init;
	mod_tail_info.fini = tail_fini;
	mod_tail_info.help = tail_help;
	mod_tail_info.run = tail_run;

	mod_tail_info.output = NULL;
	mod_tail_info.input = NULL;

	debug(NULL, "__tail_init__: Loaded mod_tail\n");

	return;
}

bot_t *tail_init(dlist_t * dlist_node, bot_t * bot)
{

	debug(bot, "tail_init: Entered\n");

	swap_inmem_get_assign_and_remove("dl_mod_tail", 0,
					 (void **)&dl_mod_tail_unique);

	return NULL;
}

bot_t *tail_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tail_fini: Entered\n");

	swap_inmem_create_and_add("dl_mod_tail", 0, dl_mod_tail_unique);

	return NULL;
}

bot_t *tail_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "tail_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help =
	    "^tail(clear:run:add:del:list) || ^tail </path/to/file>";

	return NULL;
}

bot_t *tail_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "tail_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "tail_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = MOD_TAIL_RUN;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "clear")) {
		opt = MOD_TAIL_CLEAR;
	} else if (!strncasecmp_len(dl_options_ptr, "run")) {
		opt = MOD_TAIL_RUN;
	} else if (!strncasecmp_len(dl_options_ptr, "add")) {
		opt = MOD_TAIL_ADD;
	} else if (!strncasecmp_len(dl_options_ptr, "del")) {
		opt = MOD_TAIL_DEL;
	} else if (!strncasecmp_len(dl_options_ptr, "list")) {
		opt = MOD_TAIL_LIST;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	MOD_PARSE_TOP_HALF_NODL;
	l_new_str = tail_change_string(bot, l_str_ptr, opt);
	MOD_PARSE_BOTTOM_HALF_NODL;

	return bot;
}

char *tail_change_string(bot_t * bot, char *string, int opt)
{
	unique_t *bu = NULL;
	dlist_t **dl_mod_tail = NULL;
	char *str = NULL;
	char *sep_ptr;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	bu = unique_create(bot, &dl_mod_tail_unique, UNIQUE_ID_TAG);
	if (!bu)
		return NULL;

	dl_mod_tail = (dlist_t **) & bu->data;

	if (opt == MOD_TAIL_CLEAR) {
		str = tail_op_clear(dl_mod_tail);
		return NULL;
	} else if (opt == MOD_TAIL_RUN) {
		str = tail_op_run(dl_mod_tail);
	} else if (opt == MOD_TAIL_ADD) {
		str = tail_op_add(dl_mod_tail, string);
	} else if (opt == MOD_TAIL_DEL) {
		str = tail_op_del(dl_mod_tail, string);
	} else if (opt == MOD_TAIL_LIST) {
		str = tail_op_list(dl_mod_tail);
	}

	return str;
}

char *tail_op_clear(dlist_t ** dl_mod_tail)
{

	debug(NULL, "tail_op_clear: Entered\n");

	if (!dl_mod_tail)
		return NULL;

	dlist_fini(dl_mod_tail, free);

	return NULL;
}

char *tail_op_list(dlist_t ** dl_mod_tail)
{
	dlist_t *dptr = NULL, *dl_text = NULL;
	tail_t *tail = NULL;
	char *str = NULL;
	int i = 0;

	debug(NULL, "tail_op_list: Entered\n");

	if (!dl_mod_tail)
		return NULL;

	dlist_fornext(*dl_mod_tail, dptr) {
		tail = (tail_t *) dlist_data(dptr);
		if (!tail)
			continue;

		dl_str_unite(&dl_text, "%i. %s %s", i, tail->filename,
			     tail->active > 0 ? "on" : "off");

		i++;

		if (tail->active) {
			dl_str_unite(&dl_text,
				     " dev=%i, ino=%i, mode=%i, nlink=%i, uid=%i, gid=%i, rdev=%i, size=%i, blksize=%i, blocks=%i, atime=%i, mtime=%i, ctime=%i\n",
				     (int)tail->st.st_dev, (int)tail->st.st_ino,
				     (int)tail->st.st_mode,
				     (int)tail->st.st_nlink,
				     (int)tail->st.st_uid, (int)tail->st.st_gid,
				     (int)tail->st.st_rdev,
				     (int)tail->st.st_size,
				     (int)tail->st.st_blksize,
				     (int)tail->st.st_blocks, tail->st.st_atime,
				     tail->st.st_mtime, tail->st.st_ctime);
		} else {
			dl_str_unite(&dl_text, "\n");
		}

	}

	str = dlist_to_str(dl_text);
	dl_str_destroy(&dl_text);

	return str;
}

char *tail_op_run(dlist_t ** dl_mod_tail)
{
	dlist_t *dptr, *dl_text = NULL;
	tail_t *tail = NULL;
	struct stat st;
	char *data_ptr = NULL, *str = NULL;
	int new_len = 0, n;

	debug(NULL, "tail_op_run: Entered\n");

	if (!dl_mod_tail)
		return NULL;

	dlist_fornext(*dl_mod_tail, dptr) {
		tail = (tail_t *) dlist_data(dptr);
		if (!tail)
			continue;

		n = stat(tail->filename, &st);
		if (n < 0) {
			if (tail->active)
				tail->active = 0;
			continue;
		}

		if (!tail->active) {
			tail->active = 1;
			if (!tail->st.st_size)
				memcpy(&tail->st, &st, sizeof(st));
		}

		if (tail->st.st_size > st.st_size) {
			memcpy(&tail->st, &st, sizeof(st));
		}

		new_len = st.st_size - tail->st.st_size;
		if (!new_len)
			continue;

		data_ptr = (char *)calloc(1, new_len + 1);
		if (!data_ptr)
			continue;

		n = tail_open_and_read(tail, data_ptr, new_len);
		if (n < 0) {
			free(data_ptr);
			continue;
		}

		memcpy(&tail->st, &st, sizeof(st));
		dl_str_unite(&dl_text, "%s [%s]\n", tail->filename, data_ptr);
		free(data_ptr);
	}

	str = dlist_to_str(dl_text);
	dl_str_destroy(&dl_text);

	return str;
}

char *tail_op_add(dlist_t ** dl_mod_tail, char *string)
{
	tail_t *tail = NULL;
	dlist_t *dl = NULL, *dptr = NULL;
	struct stat st;
	char *str = NULL, *str_fname = NULL;
	int n;

	debug(NULL, "tail_op_add: Entered\n");

	if (!dl_mod_tail || !string)
		return NULL;

	dl = tokenize(NULL, string, TOKENIZE_NORMAL, " ");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		str_fname = (char *)dlist_data(dptr);
		if (!str_fname)
			continue;

		tail = tail_find_node(dl_mod_tail, str_fname);
		if (tail)
			continue;

		tail = tail_init_node();
		if (!tail)
			continue;

		tail->filename = strdup(str_fname);

		n = stat(tail->filename, &st);
		if (n < 0) {
			tail->active = 0;
		} else {
			tail->active = 1;
			tail->off = tail->st.st_size;
			memcpy(&tail->st, &st, sizeof(st));
		}

		dlist_Dinsert_after(dl_mod_tail, tail);
	}

	return str;
}

char *tail_op_del(dlist_t ** dl_mod_tail, char *string)
{
	dlist_t *dptr, *dptr_tmp;
	int i = 0, num = 0;

	debug(NULL, "tail_op_del: Entered\n");

	if (!dl_mod_tail || !string)
		return NULL;

	num = atoi(string);
	if (num == 0) {
		if (string[0] != '0')
			return NULL;
	}

	dlist_fornext_retarded(*dl_mod_tail, dptr, dptr_tmp) {
		if (!dptr)
			break;

		if (i == num) {
			dlist_remove_and_free(dl_mod_tail, &dptr, tail_free);
			break;
		}

		i++;
	}

	return NULL;
}

tail_t *tail_init_node(void)
{
	tail_t *tail = NULL;

	tail = (tail_t *) calloc(1, sizeof(tail_t));
	if (!tail)
		return NULL;

	return tail;
}

tail_t *tail_find_node(dlist_t ** dl_mod_tail, char *fname)
{
	dlist_t *dptr = NULL;
	tail_t *tail = NULL;

	if (!dl_mod_tail || !fname) {
		return NULL;
	}

	dlist_fornext(*dl_mod_tail, dptr) {
		tail = (tail_t *) dlist_data(dptr);
		if (!tail)
			continue;

		if (!strcasecmp(tail->filename, fname))
			return tail;
	}

	return NULL;
}

int tail_open_and_read(tail_t * tail, char *buf, int buflen)
{
	int fd;
	int n;

	if (!tail || !buf || buflen <= 0)
		return -1;

	fd = open(tail->filename, O_RDONLY);
	if (fd <= 0)
		return -1;

	lseek(fd, tail->st.st_size, SEEK_SET);

	n = read(fd, buf, buflen);

	close(fd);

	return n;
}

void tail_free(void *arg)
{
	tail_t *tail = (tail_t *) arg;

	if (!tail)
		return;

	if (tail->filename)
		free(tail->filename);

	free(tail);

	return;
}
