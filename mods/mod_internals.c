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
#include "mod_internals.h"

void
__internals_init__ (void)
{

  strlcpy_buf (mod_internals_info.name, "mod_internals");
  strlcpy_buf (mod_internals_info.trigger, "^internals");

  mod_internals_info.init = internals_init;
  mod_internals_info.fini = internals_fini;
  mod_internals_info.help = internals_help;
  mod_internals_info.run = internals_run;


  mod_internals_info.output = NULL;
  mod_internals_info.input = NULL;


  debug (NULL, "__internals_init__: Loaded mod_internals\n");

  return;
}



bot_t *
internals_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "internals_init: Entered\n");
  return NULL;
}

bot_t *
internals_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "internals_fini: Entered\n");
  return NULL;
}

bot_t *
internals_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "internals_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help =
    "^internals || ^internals(all:in_sz:out_sz:in_strlen:out_strlen:in_data:trig_prefix)";

  return NULL;
}

bot_t *
internals_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "internals_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "internals_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  if (strcasestr (dl_options_ptr, "all"))
    {
      opt = ~opt;
    }
  else
    {
      if (strcasestr (dl_options_ptr, "in_sz"))
	{
	  opt |= MOD_INTERNALS_OPT_IN_SZ;
	}

      if (strcasestr (dl_options_ptr, "out_sz"))
	{
	  opt |= MOD_INTERNALS_OPT_OUT_SZ;
	}

      if (strcasestr (dl_options_ptr, "in_strlen"))
	{
	  opt |= MOD_INTERNALS_OPT_IN_STRLEN;
	}

      if (strcasestr (dl_options_ptr, "out_strlen"))
	{
	  opt |= MOD_INTERNALS_OPT_OUT_STRLEN;
	}

      if (strcasestr (dl_options_ptr, "in_data"))
	{
	  opt |= MOD_INTERNALS_OPT_IN_DATA;
	}

      if (strcasestr (dl_options_ptr, "trig_prefix"))
	{
	  opt |= MOD_INTERNALS_OPT_TRIG_PREFIX;
	}

      if (strcasestr (dl_options_ptr, "getpid"))
	{
	  opt |= MOD_INTERNALS_OPT_GETPID;
	}

    }

  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = internals_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
internals_change_string (bot_t * bot, char *string, int opt)
{
  dlist_t *dl_text = NULL;
  char *str = NULL;

  char *sep_ptr;

  if (!string || !bot)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  if (!strlen (string))
    string = NULL;


  if (opt & MOD_INTERNALS_OPT_IN_SZ)
    {
      internals_op_in_sz (&dl_text, bot);
    }

  if (opt & MOD_INTERNALS_OPT_OUT_SZ)
    {
      internals_op_out_sz (&dl_text, bot);
    }

  if (opt & MOD_INTERNALS_OPT_IN_STRLEN)
    {
      internals_op_in_strlen (&dl_text, bot);
    }

  if (opt & MOD_INTERNALS_OPT_OUT_STRLEN)
    {
      internals_op_out_strlen (&dl_text, bot);
    }

  if (opt & MOD_INTERNALS_OPT_IN_DATA)
    {
      internals_op_in_data (&dl_text, bot);
    }

  if (opt & MOD_INTERNALS_OPT_TRIG_PREFIX)
    {
      internals_op_trig_prefix (&dl_text, bot, string);
    }

  if (opt & MOD_INTERNALS_OPT_GETPID)
    {
      internals_op_getpid (&dl_text, bot);
    }

  str = dlist_to_str (dl_text);

  dlist_fini (&dl_text, free);

  return str;
}




int
internals_op_in_sz (dlist_t ** dl_text, bot_t * bot)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  str = str_unite ("%sin_sz=%i", c, bot->txt_data_in_sz);
  if (!str)
    return -1;

  dlist_Dinsert_after (dl_text, str);

  return 0;
}




int
internals_op_out_sz (dlist_t ** dl_text, bot_t * bot)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  str = str_unite ("%sout_sz=%i", c, bot->txt_data_out_sz);
  if (!str)
    return -1;

  dlist_Dinsert_after (dl_text, str);

  return 0;
}




int
internals_op_in_data (dlist_t ** dl_text, bot_t * bot)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  str = str_unite ("%sin_data=%s", c, bot->txt_data_in);
  if (!str)
    return -1;

  dlist_Dinsert_after (dl_text, str);

  return 0;
}




int
internals_op_in_strlen (dlist_t ** dl_text, bot_t * bot)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  str = str_unite ("%sin_strlen=%i", c, strlen (bot->txt_data_in));
  if (!str)
    return -1;

  dlist_Dinsert_after (dl_text, str);

  return 0;
}





int
internals_op_out_strlen (dlist_t ** dl_text, bot_t * bot)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  str = str_unite ("%sout_strlen=%i", c, strlen (bot->txt_data_out));
  if (!str)
    return -1;

  dlist_Dinsert_after (dl_text, str);

  return 0;
}



int
internals_op_getpid (dlist_t ** dl_text, bot_t * bot)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  str = str_unite ("%sgetpid=%i", c, getpid ());
  if (!str)
    return -1;

  dlist_Dinsert_after (dl_text, str);

  return 0;
}



int
internals_op_trig_prefix (dlist_t ** dl_text, bot_t * bot, char *trig_prefix)
{
  char *str = NULL;
  char *c;

  if (!dl_text || !bot)
    return -1;

  if (dlist_size (*dl_text) > 0)
    c = " ";
  else
    c = "";

  if (!sNULL (trig_prefix))
    {
      str = str_unite ("%strig_prefix=%s", c, bot->trig_prefix);
      if (!str)
	return -1;

      dlist_Dinsert_after (dl_text, str);
    }
  else
    {
      if (sNULL (bot->trig_prefix))
	bz (bot->trig_prefix);
      strlcpy_buf (bot->trig_prefix, trig_prefix);
    }


  return 0;
}
