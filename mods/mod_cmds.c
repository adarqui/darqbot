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
#include "mod_cmds.h"

void
__cmds_init__ (void)
{

  strlcpy_buf (mod_cmds_info.name, "mod_cmds");
  strlcpy_buf (mod_cmds_info.trigger, "^cmds");

  mod_cmds_info.init = cmds_init;
  mod_cmds_info.fini = cmds_fini;
  mod_cmds_info.help = cmds_help;
  mod_cmds_info.run = cmds_run;


  mod_cmds_info.output = NULL;
  mod_cmds_info.input = NULL;


  debug (NULL, "__cmds_init__: Loaded mod_cmds\n");

  return;
}



bot_t *
cmds_init (dlist_t * dlist_node, bot_t * bot)
{

  debug (bot, "cmds_init: Entered\n");

  return NULL;
}

bot_t *
cmds_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "cmds_fini: Entered\n");

  return NULL;
}

bot_t *
cmds_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "cmds_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^cmds || ^cmds(size:get:list) ...";

  return NULL;
}

bot_t *
cmds_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "cmds_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "cmds_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = MOD_CMDS_GET;

  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "size"))
    {
      opt = MOD_CMDS_SIZE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "get"))
    {
      opt = MOD_CMDS_GET;
    }
  else if (!strncasecmp_len (dl_options_ptr, "list"))
    {
      opt = MOD_CMDS_LIST;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = cmds_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
cmds_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  if (!string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  if (opt == MOD_CMDS_SIZE)
    {
      str = str_unite ("%i", dlist_size (bot->stack_cmds));
    }
  else if (opt == MOD_CMDS_GET)
    {
      if (dlist_size (bot->stack_cmds))
	{
	  str = strdup (dlist_data (bot->stack_cmds));
	  cmds_clean_str (str);
	}
    }
  else if (opt == MOD_CMDS_LIST)
    {
      str = dlist_to_str_fix (bot->stack_cmds, NULL, "\n");
      cmds_clean_str (str);
    }


  return str;
}







void
cmds_clean_str (char *s)
{
  char *a = NULL, *p = NULL, *q = NULL;
  int i;
  if (!s)
    return;

  a = s;

  while (1)
    {
      p = strcasestr (s, "^cmds");
      if (!p)
	break;

      q = strchr (p, '|');
      if (!q)
	*p = '\0';
      else
	{
	  for (i = 0; i < q - p; i++)
	    {
	      p[i] = '\x02';
	    }
	}
      s = s + 1;
    }

  str_shrink_chars (a, "\x02");

  return;
}
