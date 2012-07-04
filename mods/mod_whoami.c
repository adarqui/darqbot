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
#include "mod_whoami.h"

void
__whoami_init__ (void)
{

strlcpy_buf(mod_whoami_info.name, "mod_whoami");
strlcpy_buf(mod_whoami_info.trigger, "^whoami");

  mod_whoami_info.init = whoami_init;
  mod_whoami_info.fini = whoami_fini;
  mod_whoami_info.help = whoami_help;
  mod_whoami_info.run = whoami_run;


  mod_whoami_info.output = NULL;
  mod_whoami_info.input = NULL;


  debug (NULL, "__whoami_init__: Loaded mod_whoami\n");

  return;
}



bot_t *
whoami_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "whoami_init: Entered\n");
  return NULL;
}

bot_t *
whoami_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "whoami_fini: Entered\n");
  return NULL;
}

bot_t *
whoami_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "whoami_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^whoami";

  return NULL;
}

bot_t *
whoami_run (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "whoami_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "whoami_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  strlcat_bot (bot->txt_data_out, bot->txt_nick);
  strlcat_bot (bot->txt_data_out, eat_whitespace (bot->dl_module_arg));

  return bot;
}
