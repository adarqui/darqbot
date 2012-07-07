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
#include "mod_save.h"

void __save_init__(void)
{

	strlcpy_buf(mod_save_info.name, "mod_save");
	strlcpy_buf(mod_save_info.trigger, "^save");

	mod_save_info.init = save_init;
	mod_save_info.fini = save_fini;
	mod_save_info.help = save_help;
	mod_save_info.run = save_run;

	mod_save_info.output = NULL;
	mod_save_info.input = NULL;

	debug(NULL, "__save_init__: Loaded mod_save\n");

	return;
}

bot_t *save_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "save_init: Entered\n");
	return NULL;
}

bot_t *save_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "save_fini: Entered\n");
	return NULL;
}

bot_t *save_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "save_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^save";

	return NULL;
}

bot_t *save_run(dlist_t * dlist_node, bot_t * bot)
{
	char *dl_module_arg_after_options, *dl_options_ptr, *new_str, *opt_str =
	    NULL;
	int opt;

	debug(bot, "save_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "save_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	opt = 0;
	new_str = NULL;
	opt_str = NULL;

	MOD_OPTIONS_TOP_HALF;
	opt_str = dl_options_ptr;
	MOD_OPTIONS_BOTTOM_HALF;

	if (!opt_str)
		return NULL;

	new_str = save_change_string(bot, opt_str);
	if (new_str) {
		strlcat_bot(bot->txt_data_out, new_str);
		free(new_str);
		new_str = NULL;
	}

	return bot;
}

char *save_change_string(bot_t * bot, char *opt_str)
{
	bot_t *bot_new;
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	debug(bot, "save_change_string: %s\n", opt_str);

	if (!bot)
		return NULL;

	if (opt_str) {
		bot_new = bot_find_tag(opt_str);
		if (!bot_new)
			return NULL;
		bot = bot_new;
	}

	memset(buf, 0, sizeof(buf));

	strlcatfmt_bot(buf, "%s saved.", bot->tag);
	save_bot(bot);

	if (sNULL(buf) != NULL)
		str = strdup(buf);

	return str;
}

void save_bot(bot_t * bot)
{
	dlist_t *dptr;
	FILE *fp = stdout;
	server_t *server = NULL;
	nick_t *nick = NULL;
	channel_t *channel = NULL;
	char *str;

	if (!bot)
		return;

/* fp = fopen .. */
	fp = fopen(bot->conffile, "w");
	if (!fp) {
		str =
		    str_unite_static("%s/tmp/%s.conf.%i", bot->confdir,
				     bot->tag, getpid());
		fp = fopen(str, "w");
		if (!fp)
			fp = stdout;
	}

/* XXX 
  fprintf (fp, "# ^save generated conf\n");
  fprintf (fp, "# confdir=%s, conffile=%s\n", bot->confdir, bot->conffile);

  fprintf (fp, "%s\n", bot->on ? "+on" : "+off");
  fprintf (fp, "+tag %s\n", bot->tag);
  fprintf (fp, "+type %s\n", conf_get_type (bot->type));
  fprintf (fp, "+master %s\n", bot->master);
  fprintf (fp, "+proto %s\n", conf_get_proto (bot->proto));

  if (bot->logfile)
    fprintf (fp, "+logfile %s\n", bot->logfile);

  if (bot->dl_server)
    {
      fprintf (fp, "+server ");
      dlist_fornext (bot->dl_server, dptr)
      {
	server = (server_t *) dlist_data (dptr);

	fprintf (fp, "%s:%i , ", server->host, server->port);

      }

      fprintf (fp, "\n");
    }

  if (bot->dl_nick)
    {
      fprintf (fp, "+nick ");
      dlist_fornext (bot->dl_nick, dptr)
      {
	nick = (nick_t *) dlist_data (dptr);
	fprintf (fp, "%s:%s:%s:%s:%s , ", nick->nick, nick->username,
		 nick->hostname, nick->servername, nick->realname);
      }
      fprintf (fp, "\n");
    }

  fprintf (fp, "+%s\n", bot->mute ? "mute" : "unmute");

  if (bot->dl_channel)
    {
      fprintf (fp, "+channel ");
      dlist_fornext (bot->dl_channel, dptr)
      {
	channel = (channel_t *) dlist_data (dptr);
	fprintf (fp, "%s", channel->name);
	if (channel->key)
	  {
	    fprintf (fp, ":%s", channel->key);
	  }
	fprintf (fp, " , ");
      }
      fprintf (fp, "\n");
    }

  if (bot->dl_pmodules)
    {
      fprintf (fp, "+pmodules ");
      dlist_fornext (bot->dl_pmodules, dptr)
      {
	fprintf (fp, "%s , ", (char *) dlist_data (dptr));
      }
      fprintf (fp, "\n");
    }

  fprintf (fp, "+var_multi=%i\n", bot->var_multi);
  fprintf (fp, "+var_usleep=%i\n", bot->var_usleep);
  fprintf (fp, "+var_mabufsz=%i\n", bot->var_maxbufsz);
  fprintf (fp, "+var_bufsz=%i\n", bot->var_bufsz);
  fprintf (fp, "+var_maxflood=%i\n", bot->var_maxflood);
  fprintf (fp, "+var_maxnesting=%i\n", bot->var_maxnesting);
  fprintf (fp, "+var_nesting=%i\n", bot->var_nesting);
  fprintf (fp, "+var_allowpm=%i\n", bot->var_allowpm);

  if (fp != stdout)
    {
      fclose (fp);
    }

*/

	return;
}
