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
#include "mod_sig.h"

void
__sig_init__ (void)
{

  strlcpy_buf (mod_sig_info.name, "mod_sig");
  strlcpy_buf (mod_sig_info.trigger, "^sig");

  mod_sig_info.init = sig_init;
  mod_sig_info.fini = sig_fini;
  mod_sig_info.help = sig_help;
  mod_sig_info.run = sig_run;


  mod_sig_info.output = NULL;
  mod_sig_info.input = NULL;


  debug (NULL, "__sig_init__: Loaded mod_sig\n");

  return;
}



bot_t *
sig_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sig_init: Entered\n");
  return NULL;
}

bot_t *
sig_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sig_fini: Entered\n");
  return NULL;
}

bot_t *
sig_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sig_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^sig <num>";

  return NULL;
}

bot_t *
sig_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "sig_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "sig_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = sig_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}



char *
sig_change_string (char *string, int opt)
{
  char *str = NULL;

  int signum = 0;
  pid_t pid;

  char *sep_ptr;

  if (!string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  pid = getpid ();

  signum = atoi (string);
  if (signum)
    goto cleanup;

  signum = sig_stoi (string);

cleanup:
  if (signum)
    kill (pid, signum);

  return str;
}
