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
#include "mod_auto_rejoin.h"
#include "gmod_girc.h"

void __auto_rejoin_init__(void)
{

	strlcpy_buf(mod_auto_rejoin_info.name, "mod_auto_rejoin");
	strlcpy_buf(mod_auto_rejoin_info.trigger, "^auto_rejoin");

	mod_auto_rejoin_info.init = mod_auto_rejoin_init;
	mod_auto_rejoin_info.fini = mod_auto_rejoin_fini;
	mod_auto_rejoin_info.help = mod_auto_rejoin_help;
	mod_auto_rejoin_info.run = mod_auto_rejoin_run;
	mod_auto_rejoin_info.output = mod_auto_rejoin_output;
	mod_auto_rejoin_info.input = mod_auto_rejoin_input;

	debug(NULL, "__auto_rejoin_init__: Loaded mod_auto_rejoin\n");

	return;
}

bot_t *mod_auto_rejoin_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "mod_auto_rejoin_init: Entered\n");
	return NULL;
}

bot_t *mod_auto_rejoin_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "mod_auto_rejoin_fini: Entered\n");

	return NULL;
}

bot_t *mod_auto_rejoin_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "mod_auto_rejoin_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^auto_rejoin";

	return NULL;
}

bot_t *mod_auto_rejoin_run(dlist_t * dlist_node, bot_t * bot)
{

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot, "mod_auto_rejoin_run: Entered\n");

	debug(bot,
	      "mod_auto_rejoin_run: Entered: initial hook buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	return bot;
}

bot_t *mod_auto_rejoin_output(dlist_t * dlist_node, bot_t * bot)
{

	if (!dlist_node || !bot)
		return NULL;

	debug(bot, "mod_auto_rejoin_output: Entered\n");

	return NULL;
}

bot_t *mod_auto_rejoin_input(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	girc_t *girc = NULL;
	nick_t *nick;
	char *kicked_nick, *out_dup;

	if (!dlist_node || !bot)
		return NULL;

	debug(bot, "mod_auto_rejoin_input: Entered\n");

	if (bot->isline != BOT_LINE_KICK)
		return NULL;

	out_dup = strdup(bot->txt_data_in);
	debug(bot, "mod_auto_rejoin_input: out_dup=%s\n", out_dup);

	kicked_nick = strtok(out_dup, " ");
	if (!kicked_nick) {
		free(out_dup);
		return NULL;
	}

	dptr_gmod = gmodule_find_latest_gmod_dptr(bot, "^girc", NULL);
	if (!dptr_gmod) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		return NULL;
	}

	girc = (girc_t *) gmod->data;

	if (!girc) {
		return NULL;
	}

	nick = (nick_t *) dlist_data(girc->dl_nick_cur);
	if (!nick->nick) {
		free(out_dup);
		return NULL;
	}

	debug(bot, "mod_auto_rejoin_input: nick=[%s], kicked_nick=[%s]\n",
	      nick->nick, kicked_nick);

	if (!strcasecmp(nick->nick, kicked_nick)) {
/* we were kicked */

		snprintf_buf(bot->txt_data_out, "JOIN %s\n", bot->txt_to);
		bot->txt_data_out_sz = strlen(bot->txt_data_out);

		bot->shouldsend = 1;
		bot->isprivmsg = 0;

		debug(bot, "mod_auto_rejoin_input: rejoining %s, raw data=%s\n",
		      bot->txt_to, bot->txt_data_out);

		gmodule_down(bot->dl_gmodules_cur, bot);

		bot_line_clear(bot);

	}

	free(out_dup);

	return NULL;
}
