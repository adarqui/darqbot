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
#include "gmod_gfdpassc.h"

void __gfdpassc_init__(void) __attribute__ ((constructor));

void __gfdpassc_init__(void)
{

	strlcpy_buf(gmod_gfdpassc_info.name, "gmod_gfdpassc");
	strlcpy_buf(gmod_gfdpassc_info.trigger, "^gfdpassc");

	gmod_gfdpassc_info.init = gfdpassc_init;
	gmod_gfdpassc_info.fini = gfdpassc_fini;
	gmod_gfdpassc_info.help = gfdpassc_help;
	gmod_gfdpassc_info.run = gfdpassc_run;

	gmod_gfdpassc_info.off = gfdpassc_off;
	gmod_gfdpassc_info.destroy_up = gfdpassc_destroy_up;
	gmod_gfdpassc_info.destroy_down = gfdpassc_destroy_down;

	gmod_gfdpassc_info.control_up = gfdpassc_control_up;
	gmod_gfdpassc_info.control_down = gfdpassc_control_down;

	gmod_gfdpassc_info.output = gfdpassc_output;
	gmod_gfdpassc_info.input = gfdpassc_input;

	gmod_gfdpassc_info.self = &gmod_gfdpassc_info;

	debug(NULL, "__gfdpassc_init__: Loaded gmod_gfdpassc\n");

	return;
}

bot_t *gfdpassc_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gfdpassc_init: Entered\n");
	return NULL;
}

bot_t *gfdpassc_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gfdpassc_fini: Entered\n");
	return NULL;
}

bot_t *gfdpassc_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassc_t *gfdpassc = NULL;

	debug(bot, "gfdpassc_off: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	gfdpassc = (gfdpassc_t *) gmod->data;
	if (!gfdpassc) {
		return NULL;
	}

	free(gfdpassc);

	return NULL;
}

bot_t *gfdpassc_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gfdpassc_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gfdpassc";

	return NULL;
}

bot_t *gfdpassc_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gfdpassc_t *gfdpassc = NULL;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gfdpassc_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gfdpassc_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gfdpassc_info.trigger,
					       gmod_gfdpassc_info.trigger_ext);
	if (!dptr_gmod)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gfdpassc_run: GFDPASSC GMOD=NULL\n");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL,
		      "gfdpassc_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
		gfdpassc_gmod_init(bot, gmod, dptr_gmod);
		gfdpassc = gmod->data;
	}

	MOD_OPTIONS_TOP_HALF;

	gfdpassc_process_options(gfdpassc, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

/*
	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);
*/

	gfdpassc_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *gfdpassc_process_options(gfdpassc_t * gfdpassc, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];
	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gfdpassc_process_options: Entered\n");

	if (!gfdpassc || !string)
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
		gfdpassc_process_options_parse(gfdpassc,
					       (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gfdpassc_process_options_parse(gfdpassc_t * gfdpassc, char *string)
{

	if (!gfdpassc || !string)
		return;

	debug(NULL, "gfdpassc_process_options_parse: Entered\n");

	if (!strcasecmp(string, "null")) {
		gfdpassc_process_options_parse_null(gfdpassc,
						    &string[strlen("null=")]);
	}

	gmodule_parse_common_options(gfdpassc->bot, gfdpassc->gmod, string);

	return;
}

void gfdpassc_process_options_parse_null(gfdpassc_t * gfdpassc, char *string)
{

	if (!gfdpassc || !string)
		return;

	gfdpassc->null = 1;

	return;
}

char *gfdpassc_change_string(bot_t * bot, char *string, int opt)
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

bot_t *gfdpassc_output(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassc_t *gfdpassc = NULL;

	debug(bot, "gfdpassc_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gfdpassc = (gfdpassc_t *) gmod->data;
	if (!gfdpassc) {
		debug(NULL,
		      "gfdpassc_output: GFDPASSC_OUTPUT: gfdpassc=NULL\n");
		return NULL;
	}

	debug(NULL, "gfdpassc_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	gmodule_down(dlist_node, bot);

	return bot;
}

bot_t *gfdpassc_input(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassc_t *gfdpassc = NULL;

	debug(bot, "gfdpassc_input: Entered\n");

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gfdpassc = (gfdpassc_t *) gmod->data;
	if (!gfdpassc) {
		debug(NULL, "GFDPASSC_INPUT: gfdpassc=NULL\n");
		return NULL;
	}

/*
	gmodule_fix_data_down(bot);
	gmodule_up(dlist_node, bot);
*/
	if (!gfdpassc->initialized) {
		return gfdpassc_input_get(gfdpassc);
	}

	debug(NULL, "gfdpassc_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	return bot;
}

bot_t *gfdpassc_input_get(gfdpassc_t * gfdpassc)
{
	dlist_t *dptr_control = NULL;
	control_t *control = NULL;
	fdpass_control_t *fc = NULL;

	debug(NULL, "gfdpassc_input_get: Entered\n");

	if (!gfdpassc)
		return NULL;

	fc = fdpass_init(gfdpassc->gmod->trigger, gfdpassc->gmod->trigger_ext);
	if (!fc)
		return NULL;

	fdpass_req_get_raw(fc);

	fdpass_print(&fc->msg);

	control = control_init();
	control_add_sendmsg(control, &fc->msg);
	dptr_control = control_bot_add(gfdpassc->bot, control);

	gmodule_control_down(gfdpassc->dptr_gmod, gfdpassc->bot);

	control_bot_del(gfdpassc->bot, dptr_control);

	fdpass_fini(&fc);

	gfdpassc->initialized = 1;

	gmodule_up(gfdpassc->dptr_gmod, gfdpassc->bot);

	return gfdpassc->bot;
}

bot_t *gfdpassc_destroy_up_gfdpassc(gfdpassc_t * gfdpassc)
{
	bot_t *ret = NULL;

	debug(NULL, "gfdpassc_destroy_up_gfdpassc: Entered\n");

	if (gfdpassc) {
		ret = gfdpassc_destroy_up(gfdpassc->dptr_gmod, gfdpassc->bot);
	}

	return ret;
}

bot_t *gfdpassc_destroy_down_gfdpassc(gfdpassc_t * gfdpassc)
{
	bot_t *ret = NULL;

	debug(NULL, "gfdpassc_destroy_down_gfdpassc: Entered\n");

	if (gfdpassc) {
		ret = gfdpassc_destroy_down(gfdpassc->dptr_gmod, gfdpassc->bot);
	}

	return ret;
}

bot_t *gfdpassc_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gfdpassc_t *gfdpassc = NULL;

	debug(NULL, "gfdpassc_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gfdpassc = gmod->data;
			free(gfdpassc);
		}
	}

	return bot;
}

bot_t *gfdpassc_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	gfdpassc_t *gfdpassc = NULL;

	debug(NULL, "gfdpassc_destroy_up: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gfdpassc = gmod->data;
			free(gfdpassc);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void gfdpassc_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gfdpassc_t *gfdpassc = NULL;
	debug(NULL, "gfdpassc_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gfdpassc = (gfdpassc_t *) calloc(1, sizeof(gfdpassc_t));

	if (!gfdpassc)
		return;

	gmod->data = gfdpassc;

	gfdpassc->bot = bot;
	gfdpassc->dptr_gmod = dptr_gmod;
	gfdpassc->gmod = gmod;

	gfdpassc->null = 0;

	return;
}

bot_t *gfdpassc_control_up(dlist_t * dlist_node, bot_t * bot)
{
	gfdpassc_t *gfdpassc = NULL;
	bot_gmod_elm_t *gmod = NULL;
	int fd;

	debug(NULL, "gfdpassc_control_up: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	gfdpassc = (gfdpassc_t *) gmod->data;

	if (gfdpassc) {

		fd = control_get_fdpass(&bot->dl_control);
		if (fd >= 0) {

			gfdpassc->initialized = 1;
			gfdpassc->fd = fd;

			gmodule_set_can_pass_up(gmod);
			gmodule_set_can_pass_down(gmod);

			debug(NULL,
			      "gfdpassc_control_up: GFDPASSC_CONTROL_UP!!! fd found=%i\n",
			      fd);
		}
	} else {
		debug(NULL, "gfdpassc_control_up: GRAW = NULL\n");
	}

	gmodule_control_up(dlist_node, bot);

/* special: run the next gmod */
	gmodule_up(dlist_node, bot);

	return bot;
}

bot_t *gfdpassc_control_down(dlist_t * dlist_node, bot_t * bot)
{
	debug(NULL, "gfdpassc_control_down: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmodule_control_down(dlist_node, bot);

	return bot;
}

void gfdpassc_free(void *arg)
{
	gfdpassc_t *gfdpassc = (gfdpassc_t *) arg;

	if (!gfdpassc)
		return;

/* ... */

	if (!gfdpassc->gmod)
		return;

	gfdpassc->gmod->data = NULL;
	free(gfdpassc);

	return;
}
