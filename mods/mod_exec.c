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
#include "mod_exec.h"

void
__exec_init__ (void)
{

  strlcpy_buf (mod_exec_info.name, "mod_exec");
  strlcpy_buf (mod_exec_info.trigger, "^exec");

  mod_exec_info.init = exec_init;
  mod_exec_info.fini = exec_fini;
  mod_exec_info.help = exec_help;
  mod_exec_info.run = exec_run;


  mod_exec_info.output = NULL;
  mod_exec_info.input = NULL;


  debug (NULL, "__exec_init__: Loaded mod_exec\n");

  return;
}



bot_t *
exec_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "exec_init: Entered\n");
  return NULL;
}

bot_t *
exec_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "exec_fini: Entered\n");
  return NULL;
}

bot_t *
exec_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "exec_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^exec";

  return NULL;
}

bot_t *
exec_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "exec_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "exec_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = EXEC_OPT_NORMAL;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = exec_change_string (dlist_node, bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
exec_change_string (dlist_t * dlist_node, bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  if (!bot || !dlist_node || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  switch (opt)
    {
    case EXEC_OPT_NORMAL:
      {
	str = exec_op_normal (dlist_node, bot, string);
	break;
      }
    case EXEC_OPT_FORK:
      {
	str = exec_op_fork (dlist_node, bot, string);
	break;
      }
    default:
      break;
    }

  return str;
}



char *
exec_op_normal (dlist_t * dlist_node, bot_t * bot, char *string)
{
  char *str = NULL;


  if (!dlist_node || !bot || !string)
    return NULL;

  bot->txt_data_in_sz = strlen (bot->txt_data_out);

  memcpy (bot->txt_data_in, bot->txt_data_out, bot->txt_data_in_sz);
  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  pmodule_cur_run2 (bot);

/* added 04/12/2012 */
  if (sNULL (bot->txt_data_out) != NULL)
    str = strdup (bot->txt_data_out);

  return str;
}





char *
exec_op_fork (dlist_t * dlist_node, bot_t * bot, char *string)
{
  char *str = NULL;


  if (!dlist_node || !bot || !string)
    return NULL;

  bot->txt_data_in_sz = strlen (bot->txt_data_out);
  memcpy (bot->txt_data_in, bot->txt_data_out, bot->txt_data_in_sz);
  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));

  if (!bot_fork_clean (bot))
    {
      pmodule_cur_run2 (bot);

/* added 04/12/2012 */
      if (sNULL (bot->txt_data_out) != NULL)
	str = strdup (bot->txt_data_out);
    }

  return str;
}
