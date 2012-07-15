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
#include "gmod_gexec.h"

void __gexec_init__(void) __attribute__ ((constructor));

void __gexec_init__(void)
{

	strlcpy_buf(gmod_gexec_info.name, "gmod_gexec");
	strlcpy_buf(gmod_gexec_info.trigger, "^gexec");

	gmod_gexec_info.init = gexec_init;
	gmod_gexec_info.fini = gexec_fini;
	gmod_gexec_info.help = gexec_help;
	gmod_gexec_info.run = gexec_run;

	gmod_gexec_info.off = gexec_off;
	gmod_gexec_info.destroy_up = gexec_destroy_up;
	gmod_gexec_info.destroy_down = gexec_destroy_down;

	gmod_gexec_info.control_up = gexec_control_up;
	gmod_gexec_info.control_down = gexec_control_down;

	gmod_gexec_info.output = gexec_output;
	gmod_gexec_info.input = gexec_input;

	gmod_gexec_info.self = &gmod_gexec_info;

	debug(NULL, "__gexec_init__: Loaded gmod_gexec\n");

	return;
}

bot_t *gexec_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gexec_init: Entered\n");
	return NULL;
}

bot_t *gexec_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gexec_fini: Entered\n");
	return NULL;
}

bot_t *gexec_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gexec_t *gexec = NULL;

	debug(bot, "gexec_off: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	gexec = (gexec_t *) gmod->data;
	if (!gexec) {
		return NULL;
	}

	free(gexec);

	return NULL;
}

bot_t *gexec_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gexec_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gexec";

	return NULL;
}

bot_t *gexec_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gexec_t *gexec = NULL;
	int n;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gexec_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gexec_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gexec_info.trigger,
					       gmod_gexec_info.trigger_ext);
	if (!dptr_gmod)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gexec_run: GEXEC GMOD=NULL\n");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL, "gexec_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
		gexec_gmod_init(bot, gmod, dptr_gmod);
		gexec = gmod->data;
	}

	MOD_OPTIONS_TOP_HALF;

	gexec_process_options(gexec, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

	n = gexec_exec(gexec);
	if (n < 0) {
		gmodule_destroy_down(dlist_node, bot);
		return NULL;
	}

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	gexec_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *gexec_process_options(gexec_t * gexec, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gexec_process_options: Entered\n");

	if (!gexec || !string)
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
		gexec_process_options_parse(gexec, (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gexec_process_options_parse(gexec_t * gexec, char *string)
{

	if (!gexec || !string)
		return;

	debug(NULL, "gexec_process_options_parse: Entered\n");

	if (!strncasecmp_len(string, "bin=")) {
		gexec_process_options_parse_bin(gexec, &string[strlen("bin=")]);
	}

	gmodule_parse_common_options(gexec->bot, gexec->gmod, string);

	return;
}

void gexec_process_options_parse_bin(gexec_t * gexec, char *string)
{

	if (!gexec || !string)
		return;

	if (gexec->bin)
		free(gexec->bin);

	gexec->bin = strdup(string);

	return;
}

char *gexec_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	return str;
}

bot_t *gexec_output(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gexec_t *gexec = NULL;

	debug(bot, "gexec_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gexec = (gexec_t *) gmod->data;
	if (!gexec) {
		return NULL;
	}

	debug(NULL, "gexec_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	gmodule_down(dlist_node, bot);

	return bot;
}

bot_t *gexec_input(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gexec_t *gexec = NULL;
	int n;

	debug(bot, "gexec_input: Entered\n");

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gexec = (gexec_t *) gmod->data;
	if (!gexec) {
		debug(NULL, "gexec_input: GEXEC_INPUT: gexec=NULL\n");
		return NULL;
	}

/*
  gmodule_up (dlist_node, bot);
*/
	if (!gexec->initialized)
		gmodule_up(dlist_node, bot);
	else {
		n = write(gexec->pipe[1], bot->txt_data_in,
			  bot->txt_data_in_sz);
		n = write(gexec->pipe[0], bot->txt_data_in,
			  bot->txt_data_in_sz);
		n = write(gexec->pipe_b[1], bot->txt_data_in,
			  bot->txt_data_in_sz);
		debug(NULL, "gexec_input: n=%i\n", n);
	}

	debug(NULL, "gexec_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	return bot;
}

bot_t *gexec_destroy_up_gexec(gexec_t * gexec)
{
	bot_t *ret = NULL;

	debug(NULL, "gexec_destroy_up_gexec: Entered\n");

	if (gexec) {
		ret = gexec_destroy_up(gexec->dptr_gmod, gexec->bot);
	}

	return ret;
}

bot_t *gexec_destroy_down_gexec(gexec_t * gexec)
{
	bot_t *ret = NULL;

	debug(NULL, "gexec_destroy_down_gexec: Entered\n");

	if (gexec) {
		ret = gexec_destroy_down(gexec->dptr_gmod, gexec->bot);
	}

	return ret;
}

bot_t *gexec_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gexec_t *gexec = NULL;

	debug(NULL, "gexec_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gexec = gmod->data;
			free(gexec);
		}
	}

	return bot;
}

bot_t *gexec_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	gexec_t *gexec = NULL;

	debug(NULL, "gexec_destroy_up: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gexec = gmod->data;
			gexec_unset_evhooks(gexec);
			gexec_free(gexec);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void gexec_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gexec_t *gexec = NULL;
	debug(NULL, "gexec_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gexec = (gexec_t *) calloc(1, sizeof(gexec_t));

	if (!gexec)
		return;

	gmod->data = gexec;

	gexec->bot = bot;
	gexec->dptr_gmod = dptr_gmod;
	gexec->gmod = gmod;

	return;
}

bot_t *gexec_control_up(dlist_t * dlist_node, bot_t * bot)
{
	gexec_t *gexec = NULL;
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "gexec_control_up: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	gexec = (gexec_t *) gmod->data;

	gmodule_control_up(dlist_node, bot);

	return bot;
}

bot_t *gexec_control_down(dlist_t * dlist_node, bot_t * bot)
{
	debug(NULL, "gexec_control_down: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmodule_control_down(dlist_node, bot);

	return bot;
}

void gexec_free(void *arg)
{
	gexec_t *gexec = (gexec_t *) arg;

	if (!gexec)
		return;

/* ... */

	if (!gexec->gmod)
		return;

	gexec->gmod->data = NULL;

	if (gexec->bin)
		free(gexec->bin);

	gexec_unset_evhooks(gexec);

	safe_close(gexec->pipe[1]);

	free(gexec);

	return;
}

int gexec_exec(gexec_t * gexec)
{
	char **argv = NULL;
	int argc = 0;
	pid_t pid;

	if (!gexec)
		return -1;

	debug(NULL, "gexec_exec: Entered: gexec->bin=%s\n", gexec->bin);

	if (!gexec->bin)
		return -1;

	argv = tokenize_str2argv(gexec->bin, &argc, 0);
	if (!argv)
		return -1;

	if (pipe(gexec->pipe) < 0)
		goto cleanup;
	if (pipe(gexec->pipe_b) < 0)
		goto cleanup;

	pid = fork();
	if (pid < 0)
		goto cleanup;

	if (!pid) {
		gexec->pid = getpid();

		close(0);
		close(1);
		close(2);

		close(gexec->pipe_b[0]);
		dup2(gexec->pipe_b[1], 1);
		dup2(gexec->pipe_b[1], 2);
		close(gexec->pipe[1]);
		close(0);
		dup2(gexec->pipe[0], 0);

		execve(argv[0], argv, environ);
		puts("ERROR");
/* exit(0) */
		bot_fork_clean_exit(NULL);
		return 0;
	} else {
		close(gexec->pipe[0]);
		close(gexec->pipe_b[1]);
		gexec_set_evhooks(gexec);
	}

	gexec->pid = pid;
	gexec->initialized = 1;

 cleanup:
	if (argv)
		tokenize_destroy_array(NULL, argv);

	return 0;
}

int gexec_set_evhooks(gexec_t * gexec)
{

	if (!gexec)
		return -1;

	gexec->link.fd = gexec->pipe_b[0];

	safe_event_set(&gexec->link.ev, gexec->link.fd, EV_READ | EV_PERSIST,
		       gexec_evhook_link, gexec);
	safe_event_add(&gexec->link.ev, NULL);

	return 0;
}

int gexec_unset_evhooks(gexec_t * gexec)
{
	if (gexec->link.fd > 0) {
		debug(NULL, "gexec_unset_evhooks: closing: %i\n",
		      gexec->link.fd);
		safe_event_del(&gexec->link.ev);
		safe_close(gexec->link.fd);
	}

	fd_link_cleanup(&gexec->link);

	return 0;
}

void gexec_evhook_link(int fd, short event, void *arg)
{
	gexec_t *gexec = NULL;
	int n;
	char buf[MAX_BUF_SZ + 1];

	debug(NULL, "gexec_evhook_link: Entered\n");

	gexec = (gexec_t *) arg;

	if (!gexec)
		return;

	debug_set_cur(gexec->bot, gexec_evhook_link);

	waitpid(-1, &n, WNOHANG);

	bot_line_clear(gexec->bot);

	n = read(gexec->pipe_b[0], buf, sizeof(buf) - 1);

	if (n <= 0) {
		gexec_destroy_down_gexec(gexec);
		return;
	}

	memcpy(gexec->bot->txt_data_out, buf, n);
	gexec->bot->txt_data_out_sz = n;

	gmodule_down(gexec->dptr_gmod, gexec->bot);

	return;
}
