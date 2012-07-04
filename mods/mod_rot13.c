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
#include "mod_rot13.h"

void
__rot13_init__ (void)
{


strlcpy_buf(mod_rot13_info.name, "mod_rot13");
strlcpy_buf(mod_rot13_info.trigger, "^rot13");

module_add_subtrigs(&mod_rot13_info, "^caesar");

  mod_rot13_info.init = rot13_init;
  mod_rot13_info.fini = rot13_fini;
  mod_rot13_info.help = rot13_help;
  mod_rot13_info.run = rot13_run;


  mod_rot13_info.output = NULL;
  mod_rot13_info.input = NULL;

  debug (NULL, "__rot13_init__: Loaded mod_rot13\n");

  return;
}



bot_t *
rot13_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "rot13_init: Entered\n");
  return NULL;
}

bot_t *
rot13_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "rot13_fini: Entered\n");
  return NULL;
}

bot_t *
rot13_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "rot13_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^rot13 <text>";

  return NULL;
}

bot_t *
rot13_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "rot13_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "rot13_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = rot13_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;


  return bot;
}



char *
rot13_change_string (char *string, int opt)
{
  int i, c, d;


  char *sep_ptr;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  for (i = 0; i < strlen (string); i++)
    {

      c = toupper (string[i]);

      d = string[i];

      if (isalpha (c))
	{
	  if (c >= 'A' && c <= 'M')
	    {
	      d += 13;
	    }
	  else if (c >= 'N' && c <= 'Z')
	    {
	      d -= 13;
	    }
	}

      string[i] = d;

    }


  return strdup (string);
}
