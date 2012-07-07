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
#include "gmod_gnull.h"

void __gnull_init__(void) __attribute__ ((constructor));

void __gnull_init__(void)
{

	strlcpy_buf(gmod_gnull_info.name, "gmod_gnull");
	strlcpy_buf(gmod_gnull_info.trigger, "^gnull");

	gmod_gnull_info.init = gnull_init;
	gmod_gnull_info.fini = gnull_fini;
	gmod_gnull_info.help = gnull_help;
	gmod_gnull_info.run = gnull_run;

	gmod_gnull_info.off = gnull_off;
	gmod_gnull_info.destroy_up = gnull_destroy_up;
	gmod_gnull_info.destroy_down = gnull_destroy_down;

	gmod_gnull_info.control_up = gnull_control_up;
	gmod_gnull_info.control_down = gnull_control_down;

	gmod_gnull_info.output = gnull_output;
	gmod_gnull_info.input = gnull_input;

	gmod_gnull_info.self = &gmod_gnull_info;

	debug(NULL, "__gnull_init__: Loaded gmod_gnull\n");

	return;
}

bot_t *gnull_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gnull_init: Entered\n");
	return NULL;
}

bot_t *gnull_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gnull_fini: Entered\n");
	return NULL;
}

bot_t *gnull_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gnull_t *gnull = NULL;

	debug(bot, "gnull_off: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	gnull = (gnull_t *) gmod->data;
	if (!gnull) {
		return NULL;
	}

	free(gnull);

	return NULL;
}

bot_t *gnull_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gnull_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gnull";

	return NULL;
}

bot_t *gnull_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gnull_t *gnull = NULL;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gnull_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gnull_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gnull_info.trigger,
					       gmod_gnull_info.trigger_ext);
	if (!dptr_gmod)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gnull_run: GNULL GMOD=NULL\n");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL, "gnull_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
		gnull_gmod_init(bot, gmod, dptr_gmod);
		gnull = gmod->data;
	}

	MOD_OPTIONS_TOP_HALF;

	gnull_process_options(gnull, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	gnull_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *gnull_process_options(gnull_t * gnull, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];
	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gnull_process_options: Entered\n");

	if (!gnull || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	dl = tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		      "...");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		gnull_process_options_parse(gnull, (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gnull_process_options_parse(gnull_t * gnull, char *string)
{

	if (!gnull || !string)
		return;

	debug(NULL, "gnull_process_options_parse: Entered\n");

	if (!strcasecmp(string, "null")) {
		gnull_process_options_parse_null(gnull,
						 &string[strlen("null=")]);
	}

	gmodule_parse_common_options(gnull->bot, gnull->gmod, string);

	return;
}

void gnull_process_options_parse_null(gnull_t * gnull, char *string)
{

	if (!gnull || !string)
		return;

	gnull->null = 1;

	return;
}

char *gnull_change_string(bot_t * bot, char *string, int opt)
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

bot_t *gnull_output(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gnull_t *gnull = NULL;

	debug(bot, "gnull_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gnull = (gnull_t *) gmod->data;
	if (!gnull) {
		debug(NULL, "gnull_output: GNULL_OUTPUT: gnull=NULL\n");
		return NULL;
	}

	debug(NULL, "gnull_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	gmodule_down(dlist_node, bot);

	return bot;
}

bot_t *gnull_input(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gnull_t *gnull = NULL;

	debug(bot, "gnull_input: Entered\n");

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gnull = (gnull_t *) gmod->data;
	if (!gnull) {
		debug(NULL, "GNULL_INPUT: gnull=NULL\n");
		return NULL;
	}

	gmodule_fix_data_down(bot);
	gmodule_up(dlist_node, bot);

	debug(NULL, "gnull_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	return bot;
}

bot_t *gnull_destroy_up_gnull(gnull_t * gnull)
{
	bot_t *ret = NULL;

	debug(NULL, "gnull_destroy_up_gnull: Entered\n");

	if (gnull) {
		ret = gnull_destroy_up(gnull->dptr_gmod, gnull->bot);
	}

	return ret;
}

bot_t *gnull_destroy_down_gnull(gnull_t * gnull)
{
	bot_t *ret = NULL;

	debug(NULL, "gnull_destroy_down_gnull: Entered\n");

	if (gnull) {
		ret = gnull_destroy_down(gnull->dptr_gmod, gnull->bot);
	}

	return ret;
}

bot_t *gnull_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gnull_t *gnull = NULL;

	debug(NULL, "gnull_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gnull = gmod->data;
			free(gnull);
		}
	}

	return bot;
}

bot_t *gnull_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	gnull_t *gnull = NULL;

	debug(NULL, "gnull_destroy_up: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gnull = gmod->data;
			free(gnull);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void gnull_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gnull_t *gnull = NULL;
	debug(NULL, "gnull_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gnull = (gnull_t *) calloc(1, sizeof(gnull_t));

	if (!gnull)
		return;

	gmod->data = gnull;

	gnull->bot = bot;
	gnull->dptr_gmod = dptr_gmod;
	gnull->gmod = gmod;

	gnull->null = 0;

	return;
}

bot_t *gnull_control_up(dlist_t * dlist_node, bot_t * bot)
{
	gnull_t *gnull = NULL;
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "gnull_control_up: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	gnull = (gnull_t *) gmod->data;

	gmodule_control_up(dlist_node, bot);

	return bot;
}

bot_t *gnull_control_down(dlist_t * dlist_node, bot_t * bot)
{
	debug(NULL, "gnull_control_down: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmodule_control_down(dlist_node, bot);

	return bot;
}

void gnull_free(void *arg)
{
	gnull_t *gnull = (gnull_t *) arg;

	if (!gnull)
		return;

/* ... */

	if (!gnull->gmod)
		return;

	gnull->gmod->data = NULL;
	free(gnull);

	return;
}
