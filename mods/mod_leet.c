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
#include "mod_leet.h"

void
__leet_init__ (void)
{

  strlcpy_buf (mod_leet_info.name, "mod_leet");
  strlcpy_buf (mod_leet_info.trigger, "^leet");

  mod_leet_info.init = leet_init;
  mod_leet_info.fini = leet_fini;
  mod_leet_info.help = leet_help;
  mod_leet_info.run = leet_run;


  mod_leet_info.output = NULL;
  mod_leet_info.input = NULL;

  debug (NULL, "__leet_init__: Loaded mod_leet\n");

  return;
}



bot_t *
leet_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "leet_init: Entered\n");
  return NULL;
}

bot_t *
leet_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "leet_fini: Entered\n");
  return NULL;
}

bot_t *
leet_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "leet_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^leet || ^leet(0:1:2:3,0:1)";

  return NULL;
}

bot_t *
leet_run (dlist_t * dlist_node, bot_t * bot)
{
  char *str_ptr = NULL, *dl_module_arg_after_options, *dl_options_ptr;
  int opt_1, opt_2;

  debug (bot, "leet_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;


  stat_inc (bot, bot->trig_called);


  debug (bot,
	 "leet_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);



  if (bot_shouldreturn (bot))
    return NULL;



  opt_1 = 1;
  opt_2 = 0;

  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "0"))
    {
      opt_1 = 0;
    }
  else if (!strncasecmp_len (dl_options_ptr, "1"))
    {
      opt_1 = 1;
    }
  else if (!strncasecmp_len (dl_options_ptr, "2"))
    {
      opt_1 = 2;
    }
  else if (!strncasecmp_len (dl_options_ptr, "3"))
    {
      opt_1 = 3;
    }

  else
    {
      opt_1 = 1;
    }

  str_ptr = strstr (dl_options_ptr, ",");
  if (str_ptr)
    {
      str_ptr++;

      if (!strncasecmp_len (str_ptr, "0"))
	{
	  opt_2 = 0;
	}
      else if (!strncasecmp_len (str_ptr, "1"))
	{
	  opt_2 = 1;
	}
      else if (!strncasecmp_len (str_ptr, "2"))
	{
	  opt_2 = 2;
	}
      else
	{
	  opt_2 = 0;
	}
    }

  MOD_OPTIONS_BOTTOM_HALF;



  MOD_PARSE_TOP_HALF;
  l_new_str = leet_change_string (l_str_ptr, opt_1, opt_2);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}



char *
leet_change_string (char *string, int opt_1, int opt_2)
{
  int i, j, k, randval, match, upchar, needcaps;
  char *str, buf[MAX_BUF_SZ];

  char *sep_ptr;

  str = NULL;

  if (!string)
    return str;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  for (i = 0; i < strlen (string); i++)
    {
      match = 0;

      for (j = 0; leet_chars[j].c != 0; j++)
	{

	  needcaps = 0;
	  upchar = toupper (string[i]);
	  if (upchar == string[i])
	    needcaps = 1;

	  if (upchar == toupper (leet_chars[j].c))
	    {
	      if (opt_1 == 1 && leet_chars[j].n[0])
		{
/* match apply rules */

		  k = (i == 0) ? i : i - 1;

		  if (i == 0)
		    match = 0;
		  else if (string[i - 1] == ' ' && string[i] != ' ')
		    match = 0;
		  else if (string[i - 1] == 'l' && string[i] == 'i')
		    {
		      while (string[i] == 'i')
			{
			  charcat_safe (buf, string[i], sizeof (buf) - 1);
			  i++;
			}
		      match = 0;
		    }
		  else if (string[i - 1] == 'i' && string[i] == 'l')
		    {
		      while (string[i] == 'l')
			{
			  charcat_safe (buf, string[i], sizeof (buf) - 1);
			  i++;
			}
		      match = 0;
		    }
		  else if (isdigit (string[i - 1]))
		    match = 0;
		  else if (isdigit (string[i + 1]))
		    match = 0;
		  else if (!(rand () % 2))
		    match = 0;
		  else
		    match = 1;


		  if (match)
		    {
		      char *z_str = NULL;

		      if (needcaps)
			z_str = x_caps (leet_chars[j].n[0]);

		      strlcat_bot (buf,
				   z_str ==
				   NULL ? leet_chars[j].n[0] : z_str);
		      if (z_str)
			free (z_str);
		    }

		}
	      else if (opt_1 == 2 && leet_chars[j].n[0])
		{
/* match sub n[0] */

		  if (leet_chars[j].n[0])
		    {

		      char *z_str = NULL;

		      if (needcaps)
			z_str = x_caps (leet_chars[j].n[0]);

		      strlcat_bot (buf,
				   z_str ==
				   NULL ? leet_chars[j].n[0] : z_str);

		      if (z_str)
			free (z_str);
		      match = 1;
		    }

		}
	      else if (opt_1 == 3 && leet_chars[j].n[0])
		{
		  int t;

		  for (t = 0; leet_chars[j].n[t] != NULL; t++)
		    {
		    }

		  randval = rand () % t;

		  strlcat_bot (buf, leet_chars[j].n[randval]);

		  match = 1;

		}



	      else if (opt_2 == 1 && leet_chars[j].sound[0] != NULL)
		{
		  int t, u;
		  char *u_str;



		  for (t = 0; leet_chars[j].sound[t] != NULL; t++)
		    {
		    }
		  randval = rand () % t;


		  u_str = strdup (leet_chars[j].sound[randval]);
		  if (needcaps)
		    {
		      for (u = 0; u < strlen (u_str); u++)
			u_str[u] = toupper (u_str[u]);
		    }

		  strlcat_bot (buf, u_str);

		  free (u_str);
		  match = 1;

		}




	    }

	}
      if (!match)
	charcat_safe (buf, string[i], sizeof (buf) - 1);



    }


  if (sNULL (buf))
    str = strdup (buf);


  return str;
}




char *
x_caps (char *str)
{

  char *s, *t;
  if (!str)
    return NULL;

  t = s = strdup (str);
  while (*s)
    {
      *s = toupper (*s);
      s++;
    }

  return t;
}
