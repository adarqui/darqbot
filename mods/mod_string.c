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
#include "mod_string.h"

void
__string_init__ (void)
{

  strlcpy_buf (mod_string_info.name, "mod_string");
  strlcpy_buf (mod_string_info.trigger, "^string");

  module_add_subtrigs (&mod_string_info, "^strzero");
  module_add_subtrigs (&mod_string_info, "^strlen");
  module_add_subtrigs (&mod_string_info, "^strchr");
  module_add_subtrigs (&mod_string_info, "^strrchr");
  module_add_subtrigs (&mod_string_info, "^strstr");
  module_add_subtrigs (&mod_string_info, "^subchar");
  module_add_subtrigs (&mod_string_info, "^strcasestr");
  module_add_subtrigs (&mod_string_info, "^strprepend");
  module_add_subtrigs (&mod_string_info, "^index");
  module_add_subtrigs (&mod_string_info, "^rindex");
  module_add_subtrigs (&mod_string_info, "^memset");
  module_add_subtrigs (&mod_string_info, "^space");
  module_add_subtrigs (&mod_string_info, "^reverse");
  module_add_subtrigs (&mod_string_info, "^unquote");

  mod_string_info.init = string_init;
  mod_string_info.fini = string_fini;
  mod_string_info.help = string_help;
  mod_string_info.run = string_run;


  mod_string_info.output = NULL;
  mod_string_info.input = NULL;


  debug (NULL, "__string_init__: Loaded mod_string\n");

  return;
}



bot_t *
string_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "string_init: Entered\n");
  return NULL;
}

bot_t *
string_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "string_fini: Entered\n");
  return NULL;
}

bot_t *
string_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "string_help: Entered\n");


  if (!bot)
    return NULL;

/*
  if (str_match (bot->trig_called, STR_MATCH_STRCASECMP, 0, "^dice", NULL))
{
bot->dl_module_help = "^dice ::: rolls two random dice";
}
else if(str_match (bot->trig_called, STR_MATCH_STRCASECMP, 0, "^roulette", NULL))
{
bot->dl_module_help = "^roulette(numtriggerpulls) ::: pulls the \"trigger\" numtriggerpulls times and hopefully doesn't return a BANG!";
}
else {
  bot->dl_module_help = "^dumbgames ::: ^dice || ^roulette(numtriggerpulls)";
}
*/

  return NULL;
}

bot_t *
string_run (dlist_t * dlist_node, bot_t * bot)
{
  char *tok_opt = NULL;
  char *dl_module_arg_after_options, *dl_options_ptr, *opt_str = NULL;
  int opt, sub;

  debug (bot, "string_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "string_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  sub = MOD_STRING;
  opt = 0;

  if (!strcasecmp (bot->trig_called, "^strzero"))
    {
      sub = MOD_STRING_STRZERO;
    }
  else if (!strcasecmp (bot->trig_called, "^strlen"))
    {
      sub = MOD_STRING_STRLEN;
    }
  else if (!strcasecmp (bot->trig_called, "^strchr"))
    {
      sub = MOD_STRING_STRCHR;
    }
  else if (!strcasecmp (bot->trig_called, "^strrchr"))
    {
      sub = MOD_STRING_STRRCHR;
    }
  else if (!strcasecmp (bot->trig_called, "^strstr"))
    {
      sub = MOD_STRING_STRSTR;
    }
  else if (!strcasecmp (bot->trig_called, "^strcasestr"))
    {
      sub = MOD_STRING_STRCASESTR;
    }
  else if (!strcasecmp (bot->trig_called, "^strprepend"))
    {
      sub = MOD_STRING_STRPREPEND;
    }
  else if (!strcasecmp (bot->trig_called, "^index"))
    {
      sub = MOD_STRING_INDEX;
    }
  else if (!strcasecmp (bot->trig_called, "^rindex"))
    {
      sub = MOD_STRING_RINDEX;
    }
  else if (!strcasecmp (bot->trig_called, "^subchar"))
    {
      sub = MOD_STRING_SUBCHAR;
    }
  else if (!strcasecmp (bot->trig_called, "^memset"))
    {
      sub = MOD_STRING_MEMSET;
    }
  else if (!strcasecmp (bot->trig_called, "^space"))
    {
      sub = MOD_STRING_SPACE;
    }
  else if (!strcasecmp (bot->trig_called, "^reverse"))
    {
      sub = MOD_STRING_REVERSE;
    }
  else if (!strcasecmp (bot->trig_called, "^unquote"))
    {
      sub = MOD_STRING_UNQUOTE;
    }


  opt = MOD_STRING_OPT_BEFORE;

  MOD_OPTIONS_TOP_HALF;

  tok_opt = strtok (dl_options_ptr, ",");

  if (tok_opt)
    {
      if (tok_opt[0] == 'a')
	{
	  opt = MOD_STRING_OPT_AFTER;
	}
      else if (tok_opt[0] == 'b')
	{
	  opt = MOD_STRING_OPT_BEFORE;
	}
      else
	{
	  opt = 0;
	  opt_str = tok_opt;
	}

      if (!opt_str)
	opt_str = strtok (NULL, "");
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;

  switch (sub)
    {
    case MOD_STRING_STRZERO:
      {
	l_new_str = string_op_strzero (bot, l_str_ptr, opt_str, opt);
	break;
      }
    case MOD_STRING_STRLEN:
      {
	l_new_str = string_op_strlen (bot, l_str_ptr, opt_str, opt);
	break;
      }
    case MOD_STRING_STRCHR:
      {
	l_new_str = string_op_AstrAint (bot, l_str_ptr, opt_str, opt, strchr);
	break;
      }
    case MOD_STRING_STRRCHR:
      {
	l_new_str =
	  string_op_AstrAint (bot, l_str_ptr, opt_str, opt, strrchr);
	break;
      }
    case MOD_STRING_INDEX:
      {
	l_new_str = string_op_AstrAint (bot, l_str_ptr, opt_str, opt, index);
	break;
      }
    case MOD_STRING_RINDEX:
      {
	l_new_str = string_op_AstrAint (bot, l_str_ptr, opt_str, opt, rindex);
	break;
      }
    case MOD_STRING_MEMSET:
      {
	l_new_str =
	  string_op_AstrAint (bot, l_str_ptr, opt_str, opt, string_memset);
	break;
      }
    case MOD_STRING_STRSTR:
      {
	l_new_str = string_op_AstrAstr (bot, l_str_ptr, opt_str, opt, strstr);
	break;
      }
    case MOD_STRING_STRCASESTR:
      {
	l_new_str =
	  string_op_AstrAstr (bot, l_str_ptr, opt_str, opt, strcasestr);
	break;
      }
    case MOD_STRING_STRPREPEND:
      {
	l_new_str =
	  string_op_AstrAstr (bot, l_str_ptr, opt_str, opt,
			      string_strprepend);
	break;
      }
    case MOD_STRING_SUBCHAR:
      {
	l_new_str = string_op_subchar (bot, l_str_ptr, opt_str, opt);
	break;
      }
    case MOD_STRING_SPACE:
      {
	l_new_str = string_op_space (bot, l_str_ptr, opt_str, opt);
	break;
      }
    case MOD_STRING_REVERSE:
      {
	l_new_str = string_op_reverse (bot, l_str_ptr, opt_str, opt);
	break;
      }
    case MOD_STRING_UNQUOTE:
      {
	l_new_str = string_op_unquote (bot, l_str_ptr, opt_str, opt);
	break;
      }
    default:
      break;
    }

  MOD_PARSE_BOTTOM_HALF_NODL;


  return bot;
}



char *
string_op_strzero (bot_t * bot, char *string, char *opt_str, int opt)
{

  if (!bot)
    return NULL;

  strzero_bot (bot->txt_data_out);

  return NULL;
}



char *
string_op_strlen (bot_t * bot, char *string, char *opt_str, int opt)
{
  int len = 0, sign = 1, str_len = 0;
  char *ptr_base = NULL, *ptr_middle = NULL, *ptr_end = NULL, *str = NULL;

  debug (NULL, "string_op_strlen: Entered\n");

  if (!bot || !sNULL (opt_str))
    return NULL;

  len = atoi (opt_str);
  if (len >= 0)
    sign = 1;
  else
    sign = -1;

  len = abs (len);

  str_len = strlen (string);
  ptr_base = string;
  ptr_end = ptr_base + str_len;

  if ((len + ptr_base) > ptr_end)
    {
      len = str_len;
    }
  ptr_middle = ptr_base + str_len;

  switch (opt)
    {
    case MOD_STRING_OPT_BEFORE:
      {
	if (sign > 0)
	  {
	    str = ptr_base;
	    str_len = len;
	  }
	else
	  {
	    str = (ptr_end - len);
	    str_len = strlen (str);
	  }
	break;
      }
    case MOD_STRING_OPT_AFTER:
      {
	if (sign > 0)
	  {
	    str = ptr_base + len;
	    str_len = strlen (ptr_base);
	  }
	else
	  {
	    str = ptr_end - len;
	    str_len = (str - ptr_base);
	    str = ptr_base;
	  }
	break;
      }
    default:
      break;
    }

  if (str)
    {
      str = strdup_len (str, str_len);
      strzero_bot (bot->txt_data_out);
      strlcatfmt_bot (bot->txt_data_out, "%.*s", str_len, str);
      free (str);
    }

  return NULL;
}






char *
string_op_AstrAint (bot_t * bot, char *string, char *opt_str, int opt,
		    char *(*fn) (const char *, int))
{
  bot_t bot2;
  int str_len = 0;
  char *ptr_base = NULL, *ptr_middle = NULL, *ptr_end = NULL, *str = NULL;

  debug (NULL, "string_op_AstrAint: Entered\n");

  if (!bot || !sNULL (opt_str))
    return NULL;

  bz2 (bot2);
  memcpy (bot2.txt_data_out, string, sizeof (bot2.txt_data_out));

  string = bot2.txt_data_out;

  str_len = strlen (string);
  ptr_base = string;
  ptr_end = ptr_base + str_len;
  ptr_middle = ptr_base + str_len;

//str = strchr(ptr_base, opt_str[0]);
  str = fn (ptr_base, opt_str[0]);
  if (!str)
    {
      strzero_bot (bot->txt_data_out);
      return NULL;
    }

  if (str == ptr_base)
    {
      str_len = strlen (str);
    }
  else
    {
      switch (opt)
	{
	case MOD_STRING_OPT_BEFORE:
	  {
	    str_len = (str + 1) - ptr_base;
	    str = ptr_base;
	    break;
	  }
	case MOD_STRING_OPT_AFTER:
	  {
	    str_len = ptr_end - str;
	    break;
	  }
	default:
	  break;
	}
    }

  if (str)
    {
      str = strdup_len (str, str_len);
      strzero_bot (bot->txt_data_out);
      strlcatfmt_bot (bot->txt_data_out, "%.*s", str_len, str);
      free (str);
    }

  return NULL;
}









char *
string_op_AstrAstr (bot_t * bot, char *string, char *opt_str, int opt,
		    char *(*fn) (const char *, const char *))
{
  bot_t bot2;
  int str_len = 0;
  char *ptr_base = NULL, *ptr_middle = NULL, *ptr_end = NULL, *str = NULL;

  debug (NULL, "string_op_AstrAstr: Entered\n");

  if (!bot || !sNULL (opt_str) || !fn)
    return NULL;

  bz2 (bot2);
  memcpy (bot2.txt_data_out, string, sizeof (bot2.txt_data_out));

  string = bot2.txt_data_out;

  str_len = strlen (string);
  ptr_base = string;
  ptr_end = ptr_base + str_len;
  ptr_middle = ptr_base + str_len;


  str = fn (ptr_base, opt_str);
  if (!str)
    {
      strzero_bot (bot->txt_data_out);
      return NULL;
    }


  if (str == ptr_base)
    {
      str_len = strlen (str);
    }
  else
    {
      switch (opt)
	{
	case MOD_STRING_OPT_BEFORE:
	  {
	    str_len = (str + 1) - ptr_base;
	    str = ptr_base;
	    break;
	  }
	case MOD_STRING_OPT_AFTER:
	  {
	    str_len = ptr_end - str;
	    break;
	  }
	default:
	  break;
	}
    }

  if (str)
    {
      str = strdup_len (str, str_len);
      strzero_bot (bot->txt_data_out);
      strlcatfmt_bot (bot->txt_data_out, "%.*s", str_len, str);
      free (str);
    }

  return NULL;
}



char *
string_op_space (bot_t * bot, char *string, char *opt_str, int opt)
{
  int i = 0;
  char buf[MAX_BUF_SZ + 1];

  if (!bot || !string || !opt_str || opt < 0)
    return NULL;

  opt = atoi (opt_str);
  if (opt > 5)
    opt = 5;


  bz (buf);

  while (*string)
    {

      if (opt == 0 && *string == ' ')
	{
	  string++;
	  continue;
	}
      else if (opt == 0 && *string != ' ')
	{
	  charcat_safe (buf, *string, sizeof (buf) - 1);
	}
      else if (opt > 0 && *string != ' ')
	{
	  charcat_safe (buf, *string, sizeof (buf) - 1);
	  for (i = 0; i < opt; i++)
	    charcat_safe (buf, ' ', sizeof (buf) - 1);
	}

      string++;
    }


  strzero_bot (bot->txt_data_out);

  if (sNULL (buf) != NULL)
    {
      strlcat_bot (bot->txt_data_out, buf);
    }

  return NULL;
}








char *
string_op_subchar (bot_t * bot, char *string, char *opt_str, int opt)
{
  int c1 = 0, c2 = 0, len = 0, x = 0;
  char buf[MAX_BUF_SZ + 1];

  debug (NULL, "string_op_subchar: Entered\n");

  if (!bot || !string || !opt_str || opt < 0)

    c1 = subchar_get_char (opt_str, &len);
  c1 = subchar_get_char (opt_str, &len);
  c2 = subchar_get_char (&opt_str[len], &len);

  bz (buf);

  for (x = 0; x < strlen (string); x++)
    {
      if (string[x] == c1)
	string[x] = c2;
      buf[x] = string[x];
    }

  strzero_bot (bot->txt_data_out);
  strlcat_bot (bot->txt_data_out, buf);

  return NULL;
}




char *
string_op_reverse (bot_t * bot, char *string, char *opt_str, int opt)
{
  debug (NULL, "string_op_reverse: Entered\n");
  char buf[MAX_BUF_SZ + 1];
  int i = 0, j = 0;

  if (!bot || !string)
    return NULL;

  bz (buf);
  j = 0;
  for (i = strlen (string) - 1; i >= 0; i--)
    {
      buf[j] = string[i];
      j++;
    }

  strzero_bot (bot->txt_data_out);
  strlcat_bot (bot->txt_data_out, buf);

  return NULL;
}




char *
string_op_unquote (bot_t * bot, char *string, char *opt_str, int opt)
{
  debug (NULL, "string_op_unquote: Entered\n");

  if (!sNULL (string))
    return NULL;

  str_shrink_quotes (string);
  return NULL;
}




char *
string_strprepend (const char *s, const char *b)
{
/* unsafe func : relies on AstrAstr & AstrAint etc, to provide a MAX_BUF_SZ'd buffer */
  int x;

  x = strprepend_bot_safe ((char *) s, (char *) b, MAX_BUF_SZ - 1);

  printf ("OMG: %s, %s\n", s, b);

  return (char *) s;
}


char *
string_memset (const char *s, int c)
{

  memset ((char *) s, c, strlen (s));

  return (char *) s;
}




int
subchar_get_char (char *s, int *len)
{
  int c = 0;
  debug (NULL, "subchar_get_char: Entered, char=[%s]\n", s);
  if (!s || !len)
    {
      return 0;
    }
  *len = 0;
  switch (*s)
    {
    case '\\':
      {
	switch (*(s + 1))
	  {
	    printf ("=%c\n", *(s + 1));
	  case '\\':
	    {
	      c = '\\';
	      break;
	    }
	  case 'n':
	    {
	      c = '\n';
	      break;
	    }
	  case '0':
	    {
	      c = '\0';
	      break;
	    }
	  case 'r':
	    {
	      c = '\r';
	      break;
	    }
	  case 't':
	    {
	      c = '\t';
	      break;
	    }
	  default:
	    {
	      *len = 2;
	      break;
	    }
	  }
	*len = 2;
	break;
      }
    default:
      {
	c = *s;
	*len = 1;
	break;
      }
    }

  printf ("z: c=%i, len=%i\n", c, *len);
  return c;
}
