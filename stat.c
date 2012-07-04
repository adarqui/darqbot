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
#include "bot.h"

int
stat_inc (bot_t * bot, char *trig)
{
  int ret;

  if (!trig)
    return -1;

  ret = mongodb_update_key_stat (bot, "test.darqbotstat", trig, 0);
  if (ret < 0)
    {
      mongodb_init (bot);
    }

  debug (bot, "stat_inc: insert returned %i\n", ret);

  return 0;
}



int
stat_retcnt (bot_t * bot, char *trig)
{
  int ret;

  if (!trig)
    return -1;

  ret = mongodb_retrieve_key_stat (bot, "test.darqbotstat", trig);

  debug (bot, "stat_retcnt: ret is %i\n", ret);

  return ret;
}
