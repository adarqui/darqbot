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
#include "gmod_gircpong.h"

void __gircpong_init__(void) __attribute__ ((constructor));

void __gircpong_init__(void)
{

	strlcpy_buf(gmod_gircpong_info.name, "gmod_gircpong");
	strlcpy_buf(gmod_gircpong_info.trigger, "^gircpong");

	gmod_gircpong_info.init = gircpong_init;
	gmod_gircpong_info.fini = gircpong_fini;
	gmod_gircpong_info.help = gircpong_help;
	gmod_gircpong_info.run = gircpong_run;

	gmod_gircpong_info.off = gircpong_off;
	gmod_gircpong_info.destroy_up = gircpong_destroy_up;
	gmod_gircpong_info.destroy_down = gircpong_destroy_down;

	gmod_gircpong_info.control_up = gircpong_control_up;
	gmod_gircpong_info.control_down = gircpong_control_down;

	gmod_gircpong_info.output = gircpong_output;
	gmod_gircpong_info.input = gircpong_input;

	gmod_gircpong_info.self = &gmod_gircpong_info;

	debug(NULL, "__gircpong_init__: Loaded gmod_gircpong\n");

	return;
}

bot_t *gircpong_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gircpong_init: Entered\n");
	return NULL;
}

bot_t *gircpong_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gircpong_fini: Entered\n");
	return NULL;
}

bot_t *gircpong_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gircpong_t *gircpong = NULL;

	debug(bot, "gircpong_off: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	gircpong = (gircpong_t *) gmod->data;
	if (!gircpong) {
		return NULL;
	}

	free(gircpong);

	return NULL;
}

bot_t *gircpong_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gircpong_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gircpong";

	return NULL;
}

bot_t *gircpong_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gircpong_t *gircpong = NULL;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gircpong_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gircpong_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gircpong_info.trigger,
					       gmod_gircpong_info.trigger_ext);
	if (!dptr_gmod)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gircpong_run: GIRCPONG GMOD=NULL");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL,
		      "gircpong_run: GIRCD GMOD->DATA = NULL, ALLOCATING");
		gircpong_gmod_init(bot, gmod, dptr_gmod);
		gircpong = gmod->data;
	}

	MOD_OPTIONS_TOP_HALF;

	gircpong_process_options(gircpong, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	gircpong_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *gircpong_process_options(gircpong_t * gircpong, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gircpong_process_options: Entered\n");

	if (!gircpong || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	dl = tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		      "...");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		gircpong_process_options_parse(gircpong,
					       (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gircpong_process_options_parse(gircpong_t * gircpong, char *string)
{

	if (!gircpong || !string)
		return;

	debug(NULL, "gircpong_process_options_parse: Entered\n");

	if (!strcasecmp(string, "null")) {
		gircpong_process_options_parse_null(gircpong,
						    &string[strlen("null=")]);
	}

	gmodule_parse_common_options(gircpong->bot, gircpong->gmod, string);

	return;
}

void gircpong_process_options_parse_null(gircpong_t * gircpong, char *string)
{

	if (!gircpong || !string)
		return;

	gircpong->null = 1;

	return;
}

char *gircpong_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char *sep_ptr;

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	return str;
}

bot_t *gircpong_output(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dl, *dptr = NULL, *dptr_tmp = NULL;
	char *str = NULL, *ptr = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gircpong_t *gircpong = NULL;

	char *pong_reply = NULL;

	debug(bot, "gircpong_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gircpong = (gircpong_t *) gmod->data;
	if (!gircpong) {
		debug(NULL, "gircpong_output: GIRCPONG_OUTPUT: gircpong=NULL");
		return NULL;
	}
	debug(NULL, "gircpong_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);
	if (gircpong->gmod->opt_opposite) {
		dl = tokenize(NULL, bot->txt_data_out,
			      TOKENIZE_NORMAL | TOKENIZE_LEAVESEP |
			      TOKENIZE_LEAVEQUOTES | TOKENIZE_MATCHANY, "\r\n");
		if (!dl)
			goto cleanup;
		dlist_fornext_retarded(dl, dptr, dptr_tmp) {
			if (!dptr)
				break;

			ptr = (char *)dlist_data(dptr);
			if (!ptr)
				continue;

			if (strncasecmp_len(ptr, "PING"))
				continue;

			ptr[1] = 'O';

			pong_reply = strdup(ptr);

			memcopy(bot->txt_data_in, pong_reply,
				sizeof(bot->txt_data_in) - 1,
				strlen(pong_reply));
			bot->txt_data_in_sz = strlen(pong_reply);
			bot->txt_data_out_sz = 0;

			dlist_remove_and_free(&dl, &dptr, free);

			gmodule_up(dlist_node, bot);

			memcopy(bot->txt_data_out, pong_reply,
				sizeof(bot->txt_data_out) - 1,
				strlen(pong_reply));
			bot->txt_data_out_sz = strlen(pong_reply);
			bot->txt_data_in_sz = 0;

			gmodule_down(dlist_node, bot);

			free(pong_reply);
		}

		str = dlist_to_str(dl);
		if (str) {
			memcopy(bot->txt_data_out, str,
				sizeof(bot->txt_data_out) - 1, strlen(str));
			bot->txt_data_out_sz = strlen(str);
			bot->txt_data_in_sz = 0;
		}

		free(str);
		tokenize_destroy(NULL, &dl);
	}

 cleanup:
	gmodule_down(dlist_node, bot);

	return bot;
}

bot_t *gircpong_input(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dl = NULL, *dptr = NULL, *dptr_tmp;
	bot_gmod_elm_t *gmod = NULL;
	gircpong_t *gircpong = NULL;
	char *str = NULL, *ptr = NULL;

	char *pong_reply = NULL;

	debug(bot, "gircpong_input: Entered\n");

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gircpong = (gircpong_t *) gmod->data;
	if (!gircpong) {
		debug(NULL, "gircpong_input: GIRCPONG_INPUT: gircpong=NULL");
		return NULL;
	}

	if (!gircpong->gmod->opt_opposite) {
		dl = tokenize(NULL, bot->txt_data_out,
			      TOKENIZE_NORMAL | TOKENIZE_LEAVESEP |
			      TOKENIZE_LEAVEQUOTES | TOKENIZE_MATCHANY, "\r\n");
		if (!dl)
			goto cleanup;
		dlist_fornext_retarded(dl, dptr, dptr_tmp) {
			if (!dptr)
				break;

			ptr = (char *)dlist_data(dptr);
			if (!ptr)
				continue;

			if (strncasecmp_len(ptr, "PING"))
				continue;

			ptr[1] = 'O';

			pong_reply = strdup(ptr);

			memcopy(bot->txt_data_out, ptr,
				sizeof(bot->txt_data_out) - 1, strlen(ptr));
			bot->txt_data_out_sz = strlen(ptr);
			bot->txt_data_in_sz = 0;

			dlist_remove_and_free(&dl, &dptr, free);

			gmodule_down(dlist_node, bot);

			memcopy(bot->txt_data_in, pong_reply,
				sizeof(bot->txt_data_in) - 1,
				strlen(pong_reply));
			bot->txt_data_in_sz = strlen(pong_reply);
			bot->txt_data_out_sz = 0;

			gmodule_up(dlist_node, bot);

			free(pong_reply);
		}

		str = dlist_to_str(dl);
		if (str) {
			memcopy(bot->txt_data_out, str,
				sizeof(bot->txt_data_out) - 1, strlen(str));
			bot->txt_data_out_sz = strlen(str);
			bot->txt_data_in_sz = 0;
		}

		free(str);
		tokenize_destroy(NULL, &dl);
	}

 cleanup:
	gmodule_up(dlist_node, bot);

	return bot;
}

bot_t *gircpong_destroy_up_gircpong(gircpong_t * gircpong)
{
	bot_t *ret = NULL;

	debug(NULL, "gircpong_destroy_up_gircpong: Entered\n");

	if (gircpong) {

		ret = gircpong_destroy_up(gircpong->dptr_gmod, gircpong->bot);
	}

	return ret;
}

bot_t *gircpong_destroy_down_gircpong(gircpong_t * gircpong)
{
	bot_t *ret = NULL;

	debug(NULL, "gircpong_destroy_down_gircpong: Entered\n");

	if (gircpong) {
		ret = gircpong_destroy_down(gircpong->dptr_gmod, gircpong->bot);
	}

	return ret;
}

bot_t *gircpong_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gircpong_t *gircpong = NULL;

	debug(NULL, "gircpong_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gircpong = gmod->data;
			free(gircpong);
		}
	}

	return bot;
}

bot_t *gircpong_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	gircpong_t *gircpong = NULL;

	debug(NULL, "gircpong_destroy_up: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gircpong = gmod->data;
			free(gircpong);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void gircpong_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gircpong_t *gircpong = NULL;
	debug(NULL, "gircpong_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gircpong = (gircpong_t *) calloc(1, sizeof(gircpong_t));

	if (!gircpong)
		return;

	gmod->data = gircpong;

	gircpong->bot = bot;
	gircpong->dptr_gmod = dptr_gmod;
	gircpong->gmod = gmod;

	gircpong->null = 0;

	return;
}

bot_t *gircpong_control_up(dlist_t * dlist_node, bot_t * bot)
{
	gircpong_t *gircpong = NULL;
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "gircpong_control_up: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	gircpong = (gircpong_t *) gmod->data;

	gmodule_control_up(dlist_node, bot);

	return bot;
}

bot_t *gircpong_control_down(dlist_t * dlist_node, bot_t * bot)
{
	debug(NULL, "gircpong_control_down: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmodule_control_down(dlist_node, bot);

	return bot;
}

void gircpong_free(void *arg)
{
	gircpong_t *gircpong = (gircpong_t *) arg;

	if (!gircpong)
		return;

/* ... */

	if (!gircpong->gmod)
		return;

	gircpong->gmod->data = NULL;
	free(gircpong);

	return;
}
