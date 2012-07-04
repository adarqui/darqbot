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
#include "mod_caps.h"

void
__caps_init__ (void)
{

  strlcpy_buf (mod_caps_info.name, "mod_caps");
  strlcpy_buf (mod_caps_info.trigger, "^caps");

  mod_caps_info.init = caps_init;
  mod_caps_info.fini = caps_fini;
  mod_caps_info.help = caps_help;
  mod_caps_info.run = caps_run;


  mod_caps_info.output = NULL;
  mod_caps_info.input = NULL;

  debug (NULL, "__caps_init__: Loaded mod_caps\n");

  return;
}



bot_t *
caps_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "caps_init: Entered\n");
  return NULL;
}

bot_t *
caps_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "caps_fini: Entered\n");
  return NULL;
}

bot_t *
caps_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "caps_help: Entered\n");

  if (!bot)
    return NULL;

  bot->dl_module_help = "^caps || ^caps(low:altc:randc:altw:randw)";

  return NULL;
}

bot_t *
caps_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "caps_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "caps_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;


  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  if (!strncasecmp_len (dl_options_ptr, "altw"))
    opt = CAPS_ALTW;
  else if (!strncasecmp_len (dl_options_ptr, "randw"))
    opt = CAPS_RANDW;
  else if (!strncasecmp_len (dl_options_ptr, "altc"))
    opt = CAPS_ALTC;
  else if (!strncasecmp_len (dl_options_ptr, "randc"))
    opt = CAPS_RANDC;
  else if (!strncasecmp_len (dl_options_ptr, "low"))
    opt = CAPS_LOW;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = caps_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;


  return bot;
}



char *
caps_change_string (char *string, int opt)
{
  int i, x, altw, word, word_rand;
  char *sep_ptr;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  i = altw = word = word_rand = x = 0;

  for (i = 0; i < strlen (string); i++)
    {
      x++;
      if (string[i] == ' ')
	{
	  while (string[i] == ' ')
	    i++;
	  word++;
	  word_rand = rand ();
	}

      if (opt == CAPS_ALTW)
	{
	  if (!(word % 2))
	    {
	      string[i] = toupper (string[i]);

	    }
	  else
	    string[i] = tolower (string[i]);

	}
      else if (opt == CAPS_RANDW)
	{
	  if (!(word_rand % 2))
	    {
	      string[i] = toupper (string[i]);
	    }
	  else
	    {
	      string[i] = tolower (string[i]);
	    }
	}
      else if (opt == CAPS_ALTC)
	{
	  if (!(x % 2))
	    string[i] = tolower (string[i]);
	  else
	    string[i] = toupper (string[i]);
	}

      else if (opt == CAPS_RANDC)
	{
	  if (!(rand () % 2))
	    string[i] = tolower (string[i]);
	  else
	    string[i] = toupper (string[i]);
	}
      else if (opt == CAPS_LOW)
	{
	  string[i] = tolower (string[i]);
	}
      else
	string[i] = toupper (string[i]);

    }

  return strdup (string);
}
