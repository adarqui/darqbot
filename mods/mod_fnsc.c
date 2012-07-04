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
#include "mod_fnsc.h"

void
__fnsc_init__ (void)
{

strlcpy_buf(mod_fnsc_info.name, "mod_fnsc");
strlcpy_buf(mod_fnsc_info.trigger, "^fnsc");

  mod_fnsc_info.init = fnsc_init;
  mod_fnsc_info.fini = fnsc_fini;
  mod_fnsc_info.help = fnsc_help;
  mod_fnsc_info.run = fnsc_run;


  mod_fnsc_info.output = NULL;
  mod_fnsc_info.input = NULL;


  debug (NULL, "__fnsc_init__: Loaded mod_fnsc\n");

  return;
}



bot_t *
fnsc_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fnsc_init: Entered\n");
  return NULL;
}

bot_t *
fnsc_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fnsc_fini: Entered\n");
  return NULL;
}

bot_t *
fnsc_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fnsc_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^fnsc";

  return NULL;
}

bot_t *
fnsc_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "fnsc_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "fnsc_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = fnsc_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}


char *
fnsc_change_string (char *string, int opt)
{
  char *str = NULL;
  char *sep_ptr;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  return str;
}

int
c_strlen (char *str)
{
  char *str_base = (char *) str;
  while (*str != '\0')
    str++;
  return str - str_base;
}

char *
c_strchr (char *s, int c)
{
  if (!s)
    return NULL;
  while (*s)
    {
      if (*s == c)
	{
	  return s;
	}
      s++;
    }
  return NULL;
}

char *
c_strrchr (char *s, int c)
{
  char *str_base = NULL;
  if (!s)
    return NULL;
  s = s + strlen (s);
  while (s >= str_base)
    {
      if (*s == c)
	{
	  return s;
	}
      s--;
    }
  return NULL;
}


int
c_abs (int x)
{
  if (x < 0)
    return x * (-1);
  return x;
}

int
c_strcmp (char *s1, char *s2)
{
  int x;
  while (*s1 == *s2)
    {
      if (*s1 == '\0')
	return 0;
    }

  if (*s1 > *s2)
    x = *s1 - *s2;
  else
    x = *s2 - *s1;
  return x;
}

int
c_rot13 (char c)
{
  int x = 0;
  if (c >= 41 && c <= 64)
    x = 54;
  else if (c >= 68 && c <= 94)
    x = 81;

  switch (x)
    {
    case 54:
      {
	if (c > 64)
	  return (c - 13);
	else
	  return (c + 13);
	return c;
	break;
    case 81:
	if (c > 81)
	  return (c - 13);
	else
	  return (c + 13);
	return c;
      }
    }

  return 0;
}
