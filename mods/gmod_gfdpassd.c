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
#include "gmod_gfdpassd.h"

void __gfdpassd_init__(void) __attribute__ ((constructor));

void __gfdpassd_init__(void)
{

	strlcpy_buf(gmod_gfdpassd_info.name, "gmod_gfdpassd");
	strlcpy_buf(gmod_gfdpassd_info.trigger, "^gfdpassd");

	gmod_gfdpassd_info.init = gfdpassd_init;
	gmod_gfdpassd_info.fini = gfdpassd_fini;
	gmod_gfdpassd_info.help = gfdpassd_help;
	gmod_gfdpassd_info.run = gfdpassd_run;

	gmod_gfdpassd_info.off = gfdpassd_off;
	gmod_gfdpassd_info.destroy_up = gfdpassd_destroy_up;
	gmod_gfdpassd_info.destroy_down = gfdpassd_destroy_down;

	gmod_gfdpassd_info.control_up = gfdpassd_control_up;
	gmod_gfdpassd_info.control_down = gfdpassd_control_down;

	gmod_gfdpassd_info.output = gfdpassd_output;
	gmod_gfdpassd_info.input = gfdpassd_input;

	gmod_gfdpassd_info.self = &gmod_gfdpassd_info;

	debug(NULL, "__gfdpassd_init__: Loaded gmod_gfdpassd\n");

	return;
}

bot_t *gfdpassd_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gfdpassd_init: Entered\n");
	return NULL;
}

bot_t *gfdpassd_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gfdpassd_fini: Entered\n");
	return NULL;
}

bot_t *gfdpassd_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassd_t *gfdpassd = NULL;

	debug(bot, "gfdpassd_off: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	gfdpassd = (gfdpassd_t *) gmod->data;
	if (!gfdpassd) {
		return NULL;
	}

	free(gfdpassd);

	return NULL;
}

bot_t *gfdpassd_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gfdpassd_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gfdpassd";

	return NULL;
}

bot_t *gfdpassd_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gfdpassd_t *gfdpassd = NULL;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gfdpassd_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gfdpassd_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gfdpassd_info.trigger,
					       gmod_gfdpassd_info.trigger_ext);
	if (!dptr_gmod)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gfdpassd_run: GFDPASSD GMOD=NULL\n");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL,
		      "gfdpassd_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
		gfdpassd_gmod_init(bot, gmod, dptr_gmod);
		gfdpassd = gmod->data;
	}

	MOD_OPTIONS_TOP_HALF;

	gfdpassd_process_options(gfdpassd, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	gfdpassd_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *gfdpassd_process_options(gfdpassd_t * gfdpassd, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];
	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gfdpassd_process_options: Entered\n");

	if (!gfdpassd || !string)
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
		gfdpassd_process_options_parse(gfdpassd,
					       (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gfdpassd_process_options_parse(gfdpassd_t * gfdpassd, char *string)
{

	if (!gfdpassd || !string)
		return;

	debug(NULL, "gfdpassd_process_options_parse: Entered\n");

	if (!strcasecmp(string, "null")) {
		gfdpassd_process_options_parse_null(gfdpassd,
						    &string[strlen("null=")]);
	}

	gmodule_parse_common_options(gfdpassd->bot, gfdpassd->gmod, string);

	return;
}

void gfdpassd_process_options_parse_null(gfdpassd_t * gfdpassd, char *string)
{

	if (!gfdpassd || !string)
		return;

	gfdpassd->null = 1;

	return;
}

char *gfdpassd_change_string(bot_t * bot, char *string, int opt)
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

bot_t *gfdpassd_output(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassd_t *gfdpassd = NULL;

	debug(bot, "gfdpassd_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gfdpassd = (gfdpassd_t *) gmod->data;
	if (!gfdpassd) {
		debug(NULL,
		      "gfdpassd_output: GFDPASSD_OUTPUT: gfdpassd=NULL\n");
		return NULL;
	}

	debug(NULL, "gfdpassd_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	gmodule_down(dlist_node, bot);

	return bot;
}

bot_t *gfdpassd_input(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassd_t *gfdpassd = NULL;

	debug(bot, "gfdpassd_input: Entered\n");

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gfdpassd = (gfdpassd_t *) gmod->data;
	if (!gfdpassd) {
		debug(NULL, "GFDPASSD_INPUT: gfdpassd=NULL\n");
		return NULL;
	}

	gmodule_fix_data_down(bot);
	gmodule_up(dlist_node, bot);

	debug(NULL, "gfdpassd_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	return bot;
}

bot_t *gfdpassd_destroy_up_gfdpassd(gfdpassd_t * gfdpassd)
{
	bot_t *ret = NULL;

	debug(NULL, "gfdpassd_destroy_up_gfdpassd: Entered\n");

	if (gfdpassd) {
		ret = gfdpassd_destroy_up(gfdpassd->dptr_gmod, gfdpassd->bot);
	}

	return ret;
}

bot_t *gfdpassd_destroy_down_gfdpassd(gfdpassd_t * gfdpassd)
{
	bot_t *ret = NULL;

	debug(NULL, "gfdpassd_destroy_down_gfdpassd: Entered\n");

	if (gfdpassd) {
		ret = gfdpassd_destroy_down(gfdpassd->dptr_gmod, gfdpassd->bot);
	}

	return ret;
}

bot_t *gfdpassd_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassd_t *gfdpassd = NULL;

	debug(NULL, "gfdpassd_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gfdpassd = gmod->data;
			free(gfdpassd);
		}
	}

	return bot;
}

bot_t *gfdpassd_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	gfdpassd_t *gfdpassd = NULL;

	debug(NULL, "gfdpassd_destroy_up: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gfdpassd = gmod->data;
			free(gfdpassd);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void gfdpassd_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gfdpassd_t *gfdpassd = NULL;
	debug(NULL, "gfdpassd_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gfdpassd = (gfdpassd_t *) calloc(1, sizeof(gfdpassd_t));

	if (!gfdpassd)
		return;

	gmod->data = gfdpassd;

	gfdpassd->bot = bot;
	gfdpassd->dptr_gmod = dptr_gmod;
	gfdpassd->gmod = gmod;

	gfdpassd->null = 0;

	return;
}

bot_t *gfdpassd_control_up(dlist_t * dlist_node, bot_t * bot)
{
	gfdpassd_t *gfdpassd = NULL;
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "gfdpassd_control_up: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	gfdpassd = (gfdpassd_t *) gmod->data;

	gmodule_control_up(dlist_node, bot);

	return bot;
}

bot_t *gfdpassd_control_down(dlist_t * dlist_node, bot_t * bot)
{
	debug(NULL, "gfdpassd_control_down: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmodule_control_down(dlist_node, bot);

	return bot;
}

void gfdpassd_free(void *arg)
{
	gfdpassd_t *gfdpassd = (gfdpassd_t *) arg;

	if (!gfdpassd)
		return;

/* ... */

	if (!gfdpassd->gmod)
		return;

	gfdpassd->gmod->data = NULL;
	free(gfdpassd);

	return;
}
