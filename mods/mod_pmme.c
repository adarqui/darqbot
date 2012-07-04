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
#include "mod_pmme.h"

void
__pmme_init__ (void)
{

strlcpy_buf(mod_pmme_info.name, "mod_pmme");
strlcpy_buf(mod_pmme_info.trigger, "^pmme");

  mod_pmme_info.init = pmme_init;
  mod_pmme_info.fini = pmme_fini;
  mod_pmme_info.help = pmme_help;
  mod_pmme_info.run = pmme_run;


  mod_pmme_info.output = NULL;
  mod_pmme_info.input = NULL;


  debug (NULL, "__pmme_init__: Loaded mod_pmme\n");

  return;
}



bot_t *
pmme_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "pmme_init: Entered\n");
  return NULL;
}

bot_t *
pmme_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "pmme_fini: Entered\n");
  return NULL;
}

bot_t *
pmme_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "pmme_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^pmme";

  return NULL;
}

bot_t *
pmme_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "pmme_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "pmme_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
  if (l_str_ptr)
    {
      strlcpy_buf (bot->txt_to, bot->txt_nick);
      l_new_str = strdup (l_str_ptr);
    }
  MOD_PARSE_BOTTOM_HALF;

  return bot;
}
