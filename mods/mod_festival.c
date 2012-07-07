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
#include "mod_festival.h"

void __festival_init__(void)
{

	strlcpy_buf(mod_festival_info.name, "mod_festival");
	strlcpy_buf(mod_festival_info.trigger, "^festival");

	mod_festival_info.init = festival_init;
	mod_festival_info.fini = festival_fini;
	mod_festival_info.help = festival_help;
	mod_festival_info.run = festival_run;

	mod_festival_info.output = NULL;
	mod_festival_info.input = NULL;

	debug(NULL, "__festival_init__: Loaded mod_festival\n");

	return;
}

bot_t *festival_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "festival_init: Entered\n");
	return NULL;
}

bot_t *festival_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "festival_fini: Entered\n");
	return NULL;
}

bot_t *festival_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "festival_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^festival";

	return NULL;
}

bot_t *festival_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr;
	int opt;

	debug(bot, "festival_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "festival_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;

	MOD_OPTIONS_TOP_HALF;
	MOD_OPTIONS_BOTTOM_HALF;

	return bot;
}

char *festival_change_string(char *string, int opt)
{
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	char *sep_ptr;

	if (!string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	if (sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

int festival_send(bot_t * bot, char *msg, int len)
{
	int fd, n;
	if (!bot || !msg || len <= 0)
		return len;

	debug(bot, "festival_send: Entered [msg=%s]\n", msg);

	str_shrink_quotes(bot->txt_data_out);

	strprepend_bot_safe(bot->txt_data_out,
			    "(voice_cmu_us_clb_arctic_clunits)(SayText \"",
			    sizeof(bot->txt_data_out));
	strlcat_bot(bot->txt_data_out, "\")(quit)\n");

	fd = bot_network_raw_connect("localhost", 1314, 0, 0);
	if (fd < 0)
		return len;

	n = send(fd, bot->txt_data_out, strlen(bot->txt_data_out), 0);

	debug(bot, "festival_send: Just sent (res=%i)[%s]\n", n,
	      bot->txt_data_out);

/* no need, it will exit with (quit) 
bot_network_raw_disconnect(fd);
*/

	return len;
}
