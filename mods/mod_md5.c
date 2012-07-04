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
#include "mod_md5.h"

void
__md5_init__ (void)
{
  strlcpy_buf (mod_md5_info.name, "mod_md5");
  strlcpy_buf (mod_md5_info.trigger, "^md5");

  mod_md5_info.init = md5_init;
  mod_md5_info.fini = md5_fini;
  mod_md5_info.help = md5_help;
  mod_md5_info.run = md5_run;


  mod_md5_info.output = NULL;
  mod_md5_info.input = NULL;


  debug (NULL, "__md5_init__: Loaded mod_md5\n");

  return;
}



bot_t *
md5_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "md5_init: Entered\n");
  return NULL;
}

bot_t *
md5_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "md5_fini: Entered\n");
  return NULL;
}

bot_t *
md5_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "md5_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^md5 ::: md5's input or argument";

  return NULL;
}

bot_t *
md5_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "md5_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "md5_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = md5_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}



char *
md5_change_string (char *string, int opt)
{
  MD5_CTX *m;
  char *str = NULL;

  char *sep_ptr;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  if (!sNULL (string))
    return NULL;


  m = (MD5_CTX *) malloc (sizeof (MD5_CTX));
  MD5Init (m);

  str = (char *) MD5Data (string, strlen (string), NULL);

  free ((MD5_CTX *) m);

  return str;
}
