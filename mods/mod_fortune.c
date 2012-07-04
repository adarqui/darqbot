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
#include "mod_fortune.h"

void
__fortune_init__ (void)
{

  strlcpy_buf (mod_fortune_info.name, "mod_fortune");
  strlcpy_buf (mod_fortune_info.trigger, "^fortune");

  module_add_subtrigs (&mod_fortune_info, "^forkfortune");

  mod_fortune_info.init = fortune_init;
  mod_fortune_info.fini = fortune_fini;
  mod_fortune_info.help = fortune_help;
  mod_fortune_info.run = fortune_run;


  mod_fortune_info.output = NULL;
  mod_fortune_info.input = NULL;


  debug (NULL, "__fortune_init__: Loaded mod_fortune\n");

  return;
}



bot_t *
fortune_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fortune_init: Entered\n");
  return NULL;
}

bot_t *
fortune_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fortune_fini: Entered\n");
  return NULL;
}

bot_t *
fortune_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fortune_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^fortune";

  return NULL;
}

bot_t *
fortune_run (dlist_t * dlist_node, bot_t * bot)
{
  pid_t pid;
  char *new_str = NULL;
  char *dl_module_arg_after_options, *dl_options_ptr;

  int argc = 0;
  char **argv = NULL;

  char local_arg_buf[1024];

  int opt = 0;

  debug (bot, "fortune_run: Entered\n");


  if (!dlist_node || !bot)
    return NULL;

  debug (bot,
	 "fortune_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

  stat_inc (bot, bot->trig_called);



  if (bot_shouldreturn (bot))
    return NULL;

  if (!strcasecmp (bot->trig_called, "^forkfortune"))
    {
      opt = 1;
    }


  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  memset (local_arg_buf, 0, sizeof (local_arg_buf));

  strlcpy_buf (local_arg_buf, "fortune");


  if (strlen (dl_options_ptr) == 0)
    {
      strlcat_buf (local_arg_buf, "-c -a");
    }
  else
    {
      strlcat_buf (local_arg_buf, dl_options_ptr);
    }

  printf
    ("TRIG_CALLED=%s, MODULE_ARG=%s, LOCAL_ARG_BUF=%s, DL_OPTIONS_PTR=%s\n",
     bot->trig_called, &bot->dl_module_arg[0], local_arg_buf, dl_options_ptr);

  argv = tokenize_array (bot, local_arg_buf, TOKENIZE_NORMAL, " ", &argc);
  if (!argv)
    return NULL;

  if (argc <= 0)
    goto cleanup;

  if (opt)
    {
      pid = bot_fork_clean (bot);
      if (!pid)
	{

	  new_str = libfortune_main (argc, argv);
	  if (new_str)
	    {
	      strlcat_bot (bot->txt_data_out, new_str);
	      free (new_str);
	    }
	  return NULL;
	}

    }
  else
    {
      new_str = libfortune_main (argc, argv);
      if (new_str)
	{
	  strlcat_bot (bot->txt_data_out, new_str);
	  free (new_str);
	}
    }

cleanup:
  tokenize_destroy_array (bot, argv);

  return bot;
}
