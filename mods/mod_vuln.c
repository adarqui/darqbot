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
#include "mod_vuln.h"

void
__vuln_init__ (void)
{

strlcpy_buf(mod_vuln_info.name, "mod_vuln");
strlcpy_buf(mod_vuln_info.trigger, "^vuln");

  mod_vuln_info.init = vuln_init;
  mod_vuln_info.fini = vuln_fini;
  mod_vuln_info.help = vuln_help;
  mod_vuln_info.run = vuln_run;


  mod_vuln_info.output = NULL;
  mod_vuln_info.input = NULL;


  debug (NULL, "__vuln_init__: Loaded mod_vuln\n");

  return;
}



bot_t *
vuln_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "vuln_init: Entered\n");
  return NULL;
}

bot_t *
vuln_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "vuln_fini: Entered\n");
  return NULL;
}

bot_t *
vuln_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "vuln_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^vuln";

  return NULL;
}

bot_t *
vuln_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "vuln_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "vuln_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  l_new_str = vuln_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}



char *
vuln_change_string (char *string, int opt)
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

  if (sNULL(buf)!=NULL)
    str = strdup (buf);

  return str;
}






void
mod_vuln_offby1_1 (char *s)
{
  char buf[15];
  memset (buf, 0, sizeof (buf));
  strcat (buf, s);	// Final parameter should be: sizeof(buf)-1
  return;
}

void
mod_vuln_malloc_1 (char *s)
{
  char *n = malloc (sizeof (s) + 1);
  strcpy (n, s);
  return;
}

void
mod_vuln_fmtstring_1 (char *s)
{
  printf (s);
  return;
}

void
mod_vuln_overflow_1 (char *s)
{
  char buf[132];

  strcpy(buf, s);
  return;
}

/* race condition 1 */

/* overwriting function pointers */
