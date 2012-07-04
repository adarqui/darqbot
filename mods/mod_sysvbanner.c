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
#include "mod_sysvbanner.h"

void
__sysvbanner_init__ (void)
{

  strlcpy_buf (mod_sysvbanner_info.name, "mod_sysvbanner");
  strlcpy_buf (mod_sysvbanner_info.trigger, "^sysvbanner");

  mod_sysvbanner_info.init = sysvbanner_init;
  mod_sysvbanner_info.fini = sysvbanner_fini;
  mod_sysvbanner_info.help = sysvbanner_help;
  mod_sysvbanner_info.run = sysvbanner_run;


  mod_sysvbanner_info.output = NULL;
  mod_sysvbanner_info.input = NULL;


  debug (NULL, "__sysvbanner_init__: Loaded mod_sysvbanner\n");

  return;
}



bot_t *
sysvbanner_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sysvbanner_init: Entered\n");
  return NULL;
}

bot_t *
sysvbanner_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sysvbanner_fini: Entered\n");
  return NULL;
}

bot_t *
sysvbanner_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "sysvbanner_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^sysvbanner <text>";

  return NULL;
}

bot_t *
sysvbanner_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "sysvbanner_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "sysvbanner_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = sysvbanner_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}



char *
sysvbanner_change_string (char *string, int opt)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];

  int i, a, b, c, len, ind;
  char line[80];
  char *line_begin, *line_end;

  char *sep_ptr;

  int max_line_pad = 0;

  if (!string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));


  line_begin = line_end = NULL;


/* calculate first line */
  len = strlen (string);
  if (len > 10)
    len = 10;
  for (a = 0; a < 7; a++)
    {
      memset (line, 0, sizeof (line));
      charcat_safe (line, ' ', sizeof (line) - 1);
      for (b = 0; b < len; b++)
	{
	  if ((ind = (string)[b] - ' ') < 0)
	    ind = 0;
	  for (c = 0; c < 7; c++)
	    {
	      line[b * 8 + c] = glyphs[(ind / 8 * 7) + a][(ind % 8 * 7) + c];
	    }
	  line[b * 8 + 7] = ' ';
	}
      for (b = len * 8 - 1; b >= 0; b--)
	{
	  if (line[b] != ' ')
	    break;
	  line[b] = '\0';
	}
      strlcat_bot (line, " \n");

      if (strlen (line) > max_line_pad)
	max_line_pad = strlen (line);

    }

  printf ("HEH maxlinepad=%i\n", max_line_pad);

  for (i = 0; i < max_line_pad; i++)
    {
      charcat_safe (buf, ' ', sizeof (buf) - 1);
    }
  charcat_safe (buf, '\n', sizeof (buf) - 1);

  len = strlen (string);
  if (len > 10)
    len = 10;
  for (a = 0; a < 7; a++)
    {
      charcat_safe (buf, ' ', sizeof (buf) - 1);
      for (b = 0; b < len; b++)
	{
	  if ((ind = (string)[b] - ' ') < 0)
	    ind = 0;
	  for (c = 0; c < 7; c++)
	    {
	      line[b * 8 + c] = glyphs[(ind / 8 * 7) + a][(ind % 8 * 7) + c];
	    }
	  line[b * 8 + 7] = ' ';
	}
      for (b = len * 8 - 1; b >= 0; b--)
	{
	  if (line[b] != ' ')
	    break;
	  line[b] = '\0';
	}
      strlcat_bot (buf, line);
      for (i = strlen (line); i < max_line_pad - 1; i++)
	{
	  charcat_safe (buf, ' ', sizeof (buf) - 1);
	}
      charcat_safe (buf, '\n', sizeof (buf) - 1);

    }


  for (i = 0; i < max_line_pad; i++)
    {
      charcat_safe (buf, ' ', sizeof (buf) - 1);
    }
  charcat_safe (buf, '\n', sizeof (buf) - 1);


  if (sNULL (buf) != NULL)
    str = strdup (buf);

  return str;
}
