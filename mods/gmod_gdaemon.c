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
#include "gmod_gdaemon.h"


void __gdaemon_init__ (void) __attribute__ ((constructor));

void
__gdaemon_init__ (void)
{

  strlcpy_buf (gmod_gdaemon_info.name, "gmod_gdaemon");
  strlcpy_buf (gmod_gdaemon_info.trigger, "^gdaemon");

  gmod_gdaemon_info.init = gdaemon_init;
  gmod_gdaemon_info.fini = gdaemon_fini;
  gmod_gdaemon_info.help = gdaemon_help;
  gmod_gdaemon_info.run = gdaemon_run;

  gmod_gdaemon_info.off = gdaemon_off;
  gmod_gdaemon_info.destroy_up = gdaemon_destroy_up;
  gmod_gdaemon_info.destroy_down = gdaemon_destroy_down;

  gmod_gdaemon_info.control_up = gdaemon_control_up;
  gmod_gdaemon_info.control_down = gdaemon_control_down;

  gmod_gdaemon_info.output = gdaemon_output;
  gmod_gdaemon_info.input = gdaemon_input;

  gmod_gdaemon_info.self = &gmod_gdaemon_info;

  debug (NULL, "__gdaemon_init__: Loaded gmod_gdaemon\n");

  return;
}



bot_t *
gdaemon_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gdaemon_init: Entered\n");
  return NULL;
}

bot_t *
gdaemon_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gdaemon_fini: Entered\n");
  return NULL;
}


bot_t *
gdaemon_off (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gdaemon_t *gdaemon = NULL;

  debug (bot, "gdaemon_off: Entered\n");


  if (!dlist_node || !bot)
    {
      return NULL;
    }

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    {
      return NULL;
    }


  gdaemon = (gdaemon_t *) gmod->data;
  if (!gdaemon)
    {
      return NULL;
    }

  free (gdaemon);

  return NULL;
}





bot_t *
gdaemon_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gdaemon_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^gdaemon";

  return NULL;
}


bot_t *
gdaemon_run (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  gdaemon_t *gdaemon = NULL;
  pid_t pid = 0;

  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "gdaemon_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "gdaemon_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  dptr_gmod =
    (dlist_t *) gmodule_find_gmod_dptr (bot, gmod_gdaemon_info.trigger,
					gmod_gdaemon_info.trigger_ext);
  if (!dptr_gmod)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    {
      debug (NULL, "gdaemon_run: GDAEMON GMOD=NULL");
      return NULL;
    }

  if (!gmod->data)
    {
      debug (NULL, "gdaemon_run: GIRCD GMOD->DATA = NULL, ALLOCATING");
      gdaemon_gmod_init (bot, gmod, dptr_gmod);
      gdaemon = gmod->data;
    }

  MOD_OPTIONS_TOP_HALF;

  gdaemon_process_options (gdaemon, dl_options_ptr);

  MOD_OPTIONS_BOTTOM_HALF;

  gmodule_set_can_pass_up (gmod);
  gmodule_set_can_pass_down (gmod);

  gmodule_set_has_run (gmod);

  pid = bot_fork_clean (bot);
  if (!pid)
    {
      char **mod_names = NULL;
      bot_t *bots[2] = { bot, NULL };

      mod_names =
	(char **) dlist_convert_dlist_to_array (gdaemon->dl_mod_names);

      bot_daemon (gdaemon->flags, mod_names, bots, gdaemon->new_name,
		  gdaemon->fd_unix_path);

      gi->pid_child = getpid ();
    }
  else
    return bot;

  gdaemon_input (dptr_gmod, bot);

  return bot;
}



char *
gdaemon_process_options (gdaemon_t * gdaemon, char *string)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  dlist_t *dl = NULL, *dptr;

  debug (NULL, "gdaemon_process_options: Entered\n");


  if (!gdaemon || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));


  dl = tokenize (NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES, "...");
  if (!dl)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    gdaemon_process_options_parse (gdaemon, (char *) dlist_data (dptr));
  }


  tokenize_destroy (NULL, &dl);

  return str;
}






void
gdaemon_process_options_parse (gdaemon_t * gdaemon, char *string)
{

  if (!gdaemon || !string)
    return;

  debug (NULL, "gdaemon_process_options_parse: Entered\n");

  if (!strncasecmp_len (string, "noclose"))
    {
      gdaemon->flags |= BOT_DAEMON_NOCLOSE;
    }
  else if (!strncasecmp_len (string, "rmmod"))
    {
      gdaemon->flags |= BOT_DAEMON_REMOVE_MODULES;
    }
  else if (!strncasecmp_len (string, "rmbot"))
    {
      gdaemon->flags |= BOT_DAEMON_REMOVE_BOTS;
    }
  else if (!strncasecmp_len (string, "evrestart"))
    {
      gdaemon->flags |= BOT_DAEMON_EVENT_RESTART;
    }
  else if (!strncasecmp_len (string, "rename"))
    {
      if (gdaemon->new_name)
	free (gdaemon->new_name);
      gdaemon->new_name = strdup (&string[strlen ("rename=")]);
      gdaemon->flags |= BOT_DAEMON_RENAME_PROC;
    }
  else if (!strncasecmp_len (string, "dalloc"))
    {
      gdaemon->flags |= BOT_DAEMON_DESTROY_ALLOC;
    }
  else if (!strncasecmp_len (string, "mod_names"))
    {
      gdaemon_process_options_parse_mod_names (gdaemon,
					       &string[strlen
						       ("mod_names=")]);
    }
  else if (!strncasecmp_len (string, "fd_unix_path"))
    {
      gdaemon->flags |= BOT_DAEMON_UNIX_RESTART;
      gdaemon->fd_unix_path = strdup (&string[strlen ("fd_unix_path=")]);
    }
  else if (!strncasecmp_len (string, "clean_fds"))
    {
      gdaemon->flags |= BOT_DAEMON_CLEAN_FDS;
    }


  gmodule_parse_common_options (gdaemon->bot, gdaemon->gmod, string);

  return;
}




void
gdaemon_process_options_parse_mod_names (gdaemon_t * gdaemon, char *string)
{
  dlist_t *dl;

  if (!gdaemon || !string)
    return;

  dl =
    tokenize (gdaemon->bot, string,
	      TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, ",");
  if (!dl)
    return;

  gdaemon->dl_mod_names = dl;

  return;
}



char *
gdaemon_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;


  if (!bot || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  return str;
}






bot_t *
gdaemon_output (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gdaemon_t *gdaemon = NULL;

  debug (bot, "gdaemon_output: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  gdaemon = (gdaemon_t *) gmod->data;
  if (!gdaemon)
    {
      debug (NULL, "GDAEMON_OUTPUT: gdaemon=NULL\n");
      return NULL;
    }

  debug (NULL, "gdaemon_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	 bot->txt_data_out);

  gmodule_down (dlist_node, bot);

  return bot;
}


bot_t *
gdaemon_input (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gdaemon_t *gdaemon = NULL;

  debug (bot, "gdaemon_input: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  gdaemon = (gdaemon_t *) gmod->data;
  if (!gdaemon)
    {
      return NULL;
    }

  gmodule_fix_data_down (bot);
  gmodule_up (dlist_node, bot);

  debug (NULL, "gdaemon_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	 bot->txt_data_out);

  debug (NULL, "gdaemon_input: end");


  return bot;
}




bot_t *
gdaemon_destroy_up_gdaemon (gdaemon_t * gdaemon)
{
  bot_t *ret = NULL;

  debug (NULL, "gdaemon_destroy_up_gdaemon: Entered\n");

  if (gdaemon)
    {
      ret = gdaemon_destroy_up (gdaemon->dptr_gmod, gdaemon->bot);
    }

  return ret;
}


bot_t *
gdaemon_destroy_down_gdaemon (gdaemon_t * gdaemon)
{
  bot_t *ret = NULL;

  debug (NULL, "gdaemon_destroy_down_gdaemon: Entered\n");

  if (gdaemon)
    {
      ret = gdaemon_destroy_down (gdaemon->dptr_gmod, gdaemon->bot);
    }

  return ret;
}




bot_t *
gdaemon_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gdaemon_t *gdaemon = NULL;

  debug (NULL, "gdaemon_destroy_up: Entered\n");

  gmodule_destroy_up (dlist_node, bot);

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  gdaemon = gmod->data;
	  free (gdaemon);
	}
    }

  return bot;
}

bot_t *
gdaemon_destroy_down (dlist_t * dlist_node, bot_t * bot)
{

  bot_gmod_elm_t *gmod = NULL;
  gdaemon_t *gdaemon = NULL;

  debug (NULL, "gdaemon_destroy_up: Entered\n");

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  gdaemon = gmod->data;
	  free (gdaemon);
	}
    }

  gmodule_destroy_down (dlist_node, bot);

  return bot;
}






void
gdaemon_gmod_init (bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
  gdaemon_t *gdaemon = NULL;
  debug (NULL, "gdaemon_gmod_init: Entered\n");

  if (!gmod || !bot || !dptr_gmod)
    return;

  if (gmod->data)
    return;

  gdaemon = (gdaemon_t *) calloc (1, sizeof (gdaemon_t));

  if (!gdaemon)
    return;

  gmod->data = gdaemon;

  gdaemon->bot = bot;
  gdaemon->dptr_gmod = dptr_gmod;
  gdaemon->gmod = gmod;

  return;
}








bot_t *
gdaemon_control_up (dlist_t * dlist_node, bot_t * bot)
{
  gdaemon_t *gdaemon = NULL;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gdaemon_control_up: Entered\n");

  if (!dlist_node || !bot)
    {
      return NULL;
    }


  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  gdaemon = (gdaemon_t *) gmod->data;

  gmodule_control_up (dlist_node, bot);

  return bot;
}




bot_t *
gdaemon_control_down (dlist_t * dlist_node, bot_t * bot)
{
  debug (NULL, "gdaemon_control_down: Entered\n");


  if (!dlist_node || !bot)
    return NULL;

  gmodule_control_down (dlist_node, bot);

  return bot;
}
