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
#include "mod_guile.h"

void __guile_init__(void)
{

	strlcpy_buf(mod_guile_info.name, "mod_guile");
	strlcpy_buf(mod_guile_info.trigger, "^guile");

	mod_guile_info.init = guile_init;
	mod_guile_info.fini = guile_fini;
	mod_guile_info.help = guile_help;
	mod_guile_info.run = guile_run;

	mod_guile_info.output = NULL;
	mod_guile_info.input = NULL;

	debug(NULL, "__guile_init__: Loaded mod_guile\n");

	scm_init_guile();

	return;
}

bot_t *guile_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "guile_init: Entered\n");

	guile_info.ison = 1;
	guile_info.alarm = 5;

	return NULL;
}

bot_t *guile_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "guile_fini: Entered\n");
	return NULL;
}

bot_t *guile_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "guile_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^guile (scheme...)";

	return NULL;
}

bot_t *guile_run(dlist_t * dlist_node, bot_t * bot)
{

	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "guile_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	guile_info.text_list = dlist_node;
	guile_info.bot = bot;

	debug(bot,
	      "guile_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	if (xstrcasestr_len(dl_options_ptr, "on", len_options_area)) {
		guile_info.ison = 1;
	} else if (xstrcasestr_len(dl_options_ptr, "off", len_options_area)) {
		guile_info.ison = 0;
	}
	MOD_OPTIONS_BOTTOM_HALF;

	if (!guile_info.ison)
		return NULL;

	MOD_PARSE_TOP_HALF;

	guile_info.data_opt = DATA_OPT_OUT;

	if (!bot_fork_clean(bot)) {

		global_signal_exit_hooks();
		signal(SIGALRM, guile_alarm_handler);

		l_new_str = guile_change_string(bot, l_str_ptr, opt);
		if (l_new_str != NULL) {
			strlcat_bot(bot->txt_data_out, l_new_str);
			free(l_new_str);
			l_new_str = NULL;
		}
	}

	MOD_PARSE_BOTTOM_HALF;

	return bot;
}

char *guile_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	char *argv[2];
	int argc;

	if (!string || !bot)
		return NULL;

	debug(bot, "guile_change_string: Entered\n");

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	argc = 1;
	argv[0] = string;
	argv[1] = NULL;

	clean_environ();

	scm_boot_guile(argc, argv, guile_inner_main, bot);

	return str;
}

/* This is the function which gets called by scm_boot_guile after the
   Guile library is completely initialized.  */
static void guile_inner_main(void *closure, int argc, char **argv)
{
	dlist_t *dptr;
	bot_t *bot = (bot_t *) closure;

	SCM value;

	int val;
	int old_stdout;

	char buf[10024];
	char *str;

	if (!bot)
		return;

	global_signal_exit_hooks();

	memset(buf, 0, sizeof(buf));
	memset(guile_info.bot->txt_data_in, 0,
	       sizeof(guile_info.bot->txt_data_in));

	if (guile_info.data_opt == DATA_OPT_ARG)
		strlcat_bot(guile_info.bot->txt_data_in,
			    guile_info.bot->txt_data_out);

	if (!argv)
		goto cleanup;
	if (!argv[0])
		goto cleanup;

	if (pipe(guile_info.pipefds) < 0) {
		debug(guile_info.bot, "guile_inner_main: pipe failed\n");
		goto cleanup;
	}

	debug(NULL, "guile_inner_main: pipefds[0]=%i, pipefds[1]=%i\n",
	      guile_info.pipefds[0], guile_info.pipefds[1]);

	if (guile_info.pipefds[0] != 0) {
		close(0);
	}
	old_stdout = dup(1);
	close(1);
	close(2);

	dup2(guile_info.pipefds[1], 1);
	dup2(guile_info.pipefds[1], 2);

	fcntl(guile_info.pipefds[1], F_SETFL, O_NONBLOCK);
	fcntl(guile_info.pipefds[0], F_SETFL, O_NONBLOCK);

	value = scm_c_eval_string(argv[0]);

	memset(buf, 0, sizeof(buf));
	val = read(guile_info.pipefds[0], buf, sizeof(buf) - 1);

	dup2(old_stdout, 1);
	dup2(old_stdout, 2);

	if (val <= 0) {
		bot_fork_clean_exit(guile_info.bot);
	}

	bz(guile_info.bot->txt_data_in);
	strzero_bot(guile_info.bot->txt_data_out);

	if (guile_info.data_opt == DATA_OPT_OUT) {
		strlcat_bot(guile_info.bot->txt_data_in,
			    guile_info.bot->txt_data_out);
		strzero_bot(guile_info.bot->txt_data_out);
	}
	strlcat_bot(guile_info.bot->txt_data_in, buf);

 cleanup:

	guile_info.text_list = (dlist_t *) guile_info.text_list->next;
	if (guile_info.text_list) {
		dlist_fornext(guile_info.text_list, dptr) {
			str = (char *)dlist_data(dptr);
			if (!str)
				continue;

			charcat_bot(guile_info.bot->txt_data_in, '|');
			strlcat_bot(guile_info.bot->txt_data_in, str);
		}
	}

	guile_info.bot->isforked_stop = 0;

	pmodule_cur_run2(bot);

/* added 04/12/2012 */
	if (sNULL(guile_info.bot->txt_data_out) != NULL
	    && guile_info.bot->shouldsend
	    && sNULL(guile_info.bot->txt_to) != NULL) {
/* XXX
      gmodule_down (gi->pmod_cur_dlist_node, gi->pmod_cur_bot);
*/
		gmodule_down(bot->dl_gmodules_cur, bot);
	}

	bot_fork_clean_exit(guile_info.bot);

	return;
}

void guile_alarm_handler(int signum)
{
	bot_fork_clean_exit(NULL);
	return;
}
