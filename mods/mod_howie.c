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
#include "mod_howie.h"

void __howie_init__(void)
{

	strlcpy_buf(mod_howie_info.name, "mod_howie");
	strlcpy_buf(mod_howie_info.trigger, "^howie");

	mod_howie_info.init = howie_init;
	mod_howie_info.fini = howie_fini;
	mod_howie_info.help = howie_help;
	mod_howie_info.run = howie_run;

	mod_howie_info.output = NULL;
	mod_howie_info.input = NULL;

	mod_howie_info.max_nesting = 2;

	debug(NULL, "__howie_init__: Loaded mod_howie\n");

	memset(&howie_info, 0, sizeof(howie_info));

	return;
}

bot_t *howie_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "howie_init: Entered\n");

	howie_fill_info(bot);

	if (howie_info.should_on) {
		howie_run_on(bot);
		howie_info.should_on = 0;
	}

	return NULL;
}

bot_t *howie_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "howie_fini: Entered\n");

	howie_run_off(bot);

	return NULL;
}

bot_t *howie_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "howie_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^howie(on:off)";

	return NULL;
}

bot_t *howie_run(dlist_t * dlist_node, bot_t * bot)
{
	char *str_ptr, *dl_module_arg_after_options, *dl_options_ptr, *new_str;
	int opt, n;

	debug(bot, "howie_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "howie_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	if (bot->var_nesting_cur > mod_howie_info.max_nesting)
		return NULL;

	opt = 0;
	new_str = NULL;

	MOD_OPTIONS_TOP_HALF;

	if (xstrcasestr_len(dl_options_ptr, "on", len_options_area)) {
		howie_run_on(bot);
		return NULL;
	} else if (xstrcasestr_len(dl_options_ptr, "off", len_options_area)) {
		howie_run_off(bot);
		return NULL;
	}

	MOD_OPTIONS_BOTTOM_HALF;

	str_ptr = eat_whitespace(dl_module_arg_after_options);
	if (!str_ptr)
		str_ptr = "";

	if (sNULL(bot->txt_data_out) != NULL) {
		new_str =
		    str_unite("%s: %s%s\n", bot->txt_nick, bot->txt_data_out,
			      str_ptr);
	} else
		new_str = str_unite("%s: %s\n", bot->txt_nick, str_ptr);

	if (new_str) {
		char buf[1024];
		char *buf_ptr;
		if (howie_info.ison) {
			debug(bot, "howie_run: sending [%s]\n", new_str);
			write(howie_info.pipefd[1], new_str, strlen(new_str));

			usleep(50000);

			memset(buf, 0, sizeof(buf));
			n = read(howie_info.pipefd_b[0], buf, sizeof(buf) - 1);
			buf_ptr = strchr(buf, ':');
			if (buf_ptr) {
				if (strlen(buf_ptr) > 4) {
					strzero_bot(bot->txt_data_out);
					strlcat_bot(bot->txt_data_out, buf);
				} else {
					strzero_bot(bot->txt_data_out);
				}
			} else {
				strzero_bot(bot->txt_data_out);
			}

		}

		free(new_str);
	}

	return bot;
}

void howie_fill_info(bot_t * bot)
{
	int i;
	char *str, *tok;
	char buf[1024];

	if (howie_info.initialized)
		return;

	if (howie_info.fp) {

		fclose(howie_info.fp);
		howie_info.fp = NULL;
	}

	str = str_unite_static("%s/mods/mod_howie_files/conf", gi->confdir);

	if (!sNULL(str))
		goto cleanup;

	howie_info.fp = fopen(str, "r");

	if (!howie_info.fp)
		goto cleanup;

	while (1) {
		memset(buf, 0, sizeof(buf));
		if (fgets(buf, sizeof(buf) - 1, howie_info.fp) == NULL)
			break;

		strstrip_nl(buf);

		if (!strncasecmp(buf, "exec", 4)) {

/* very simple argv builder */
			tok = buf + strlen("exec ");

			howie_info.argc = 0;
			howie_info.argv = (char **)calloc(sizeof(char *), 10);

			while (1) {
				tok = strtok(tok, " ");
				if (!tok)
					break;

				howie_info.argv[howie_info.argc] = strdup(tok);
				howie_info.argc++;
				tok = NULL;
			}

		} else if (!strncasecmp(buf, "dir", 3)) {
			howie_info.dir = strdup(&buf[strlen("dir ")]);
		}

		else if (!strncasecmp(buf, "on", 2)) {
			howie_info.should_on = 1;
		} else if (!strncasecmp(buf, "off", 3)) {
			howie_info.should_on = 0;
		}

	}

	if (howie_info.argv) {
		for (i = 0; howie_info.argv[i] != NULL; i++) {
			printf("%i. %s\n", i, howie_info.argv[i]);
		}
	}

 cleanup:

	return;
}

int howie_run_on(bot_t * bot)
{
	pid_t pid;
	if (!bot)
		return -1;

	debug(bot, "howie_run_on: Entered\n");

	if (howie_info.ison) {
		debug(bot, "howie_run_on: howie is already on\n");
		return -1;
	}

	if (!howie_info.dir) {
		debug(bot, "howie_run_on: howie_info.dir = NULL\n");
		return -1;
	}

	if (pipe(howie_info.pipefd) < 0) {
		debug(bot, "howie_run_on: pipe Failed\n");
		return -1;
	}

	if (pipe(howie_info.pipefd_b) < 0) {
		debug(bot, "howie_run_on: pipe B Failed\n");
		return -1;
	}

	fcntl(howie_info.pipefd_b[0], F_SETFL, O_NONBLOCK);

	debug(NULL, "howie_run_on: pipe0=%i, pipe1=%i\n", howie_info.pipefd[0],
	      howie_info.pipefd[1]);

	if (!(pid = fork())) {

		chdir(howie_info.dir);

		close(howie_info.pipefd_b[0]);
		dup2(howie_info.pipefd_b[1], 1);
		dup2(howie_info.pipefd_b[1], 2);

		close(howie_info.pipefd[1]);
		close(0);
		dup2(howie_info.pipefd[0], 0);

		debug(bot, "-> howie_run_on: about to execve %s\n",
		      howie_info.argv[0]);

		execve(howie_info.argv[0], howie_info.argv, environ);
		perror("execve: ");
		exit(0);
	} else {
		close(howie_info.pipefd[0]);
		close(howie_info.pipefd_b[1]);

		howie_info.ison = pid;

		debug(bot, "howie_run_on: ison=%i\n", howie_info.ison);
	}

	return 0;
}

int howie_run_off(bot_t * bot)
{
	int i;

	debug(bot, "howie_run_off: Entered\n");

	if (howie_info.ison) {
		kill(howie_info.ison, SIGTERM);
		howie_info.ison = 0;
	}

	if (howie_info.fp)
		fclose(howie_info.fp);

	howie_info.fp = NULL;

	if (howie_info.argv) {
		for (i = 0; howie_info.argv[i] != NULL; i++) {
			free(howie_info.argv[i]);
		}

		free(howie_info.argv);
	}

	howie_info.argv = NULL;

	if (howie_info.dir)
		free(howie_info.dir);

	howie_info.dir = NULL;

	return 0;
}
