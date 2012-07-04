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


void
pmodule_sort (void)
{
  xmodule_sort (XMODULE_TYPE_PMODULE);
  return;
}




void
pmodule_update_stats (void)
{
  xmodule_update_stats (XMODULE_TYPE_PMODULE);
  return;
}


void
pmodule_lock (char *name)
{
  xmodule_lock (XMODULE_TYPE_PMODULE, name);
  return;
}

void
pmodule_unlock (char *name)
{
  xmodule_unlock (XMODULE_TYPE_PMODULE, name);
  return;
}


dlist_t *
pmodule_find_by_trig_dptr (char *name)
{
  return xmodule_find_by_trig_dptr (XMODULE_TYPE_PMODULE, name);
}


module_t *
pmodule_find_by_trig (char *name)
{
  return xmodule_find_by_trig (XMODULE_TYPE_PMODULE, name);
}



module_t *
pmodule_find_by_name (char *name)
{
  return xmodule_find_by_name (XMODULE_TYPE_PMODULE, name);
}



void
pmodule_update (module_t * mod_struct, char *name)
{
  xmodule_update (XMODULE_TYPE_PMODULE, mod_struct, name);
  return;
}



char *
pmodule_path (char *name)
{

  return xmodule_path (XMODULE_TYPE_PMODULE, name);
}

module_t *
pmodule_load (char *name)
{
  return xmodule_load (XMODULE_TYPE_PMODULE, name);
}


module_t *
pmodule_unload (module_t * mod)
{
  return xmodule_unload (XMODULE_TYPE_PMODULE, mod, NULL);
}



void
pmodule_iolist (void)
{
  xmodule_iolist (XMODULE_TYPE_PMODULE);
  return;
}


void
pmodule_timerlist (void)
{
  xmodule_timerlist (XMODULE_TYPE_PMODULE);
  return;
}

void
pmodule_list (void)
{
  xmodule_list (XMODULE_TYPE_PMODULE);
  return;
}


void
pmodule_load_all (void)
{

  xmodule_load_all (XMODULE_TYPE_PMODULE);

  return;
}



bot_t *
pmodule_unload_all (void)
{
  return xmodule_unload_all (XMODULE_TYPE_PMODULE);
}


/* console parsing stuff */
void
console_pmodload (char *arg)
{

  console_xmodload (XMODULE_TYPE_PMODULE, arg);

  return;
}


void
console_pmodunload (char *arg)
{

  console_xmodunload (XMODULE_TYPE_PMODULE, arg);
  return;
}

void
console_pmodreload (char *arg)
{

  console_xmodreload (XMODULE_TYPE_PMODULE, arg);

  return;
}



void
pmodule_cur_set (bot_t * (*fn) (dlist_t *, bot_t *), dlist_t * dlist_node,
		 bot_t * bot)
{


  debug (bot, "pmodule_cur_set: Entered\n");
  if (!fn || !dlist_node || !bot)
    return;

  gi->pmod_cur = fn;
  gi->pmod_cur_dlist_node = dlist_node;
  gi->pmod_cur_bot = bot;

  return;
}


void
pmodule_cur_clear (void)
{
  debug (NULL, "pmodule_cur_clear: Entered\n");

  gi->pmod_cur = NULL;
  gi->pmod_cur_dlist_node = NULL;
  gi->pmod_cur_bot = NULL;

  return;
}


bot_t *
pmodule_cur_run (void)
{

  debug (NULL, "pmodule_ur_run: Entered\n");

  if (gi->pmod_cur && gi->pmod_cur_dlist_node && gi->pmod_cur_bot)
    {
      return gi->pmod_cur (gi->pmod_cur_dlist_node, gi->pmod_cur_bot);
    }

  return NULL;
}


bot_t *
pmodule_cur_run2 (bot_t * bot)
{

  debug (NULL, "pmodule_cur_run2: Entered\n");

  if (gi->pmod_cur && gi->pmod_cur_dlist_node && bot)
    {
      return gi->pmod_cur (gi->pmod_cur_dlist_node, bot);
    }

  return NULL;
}
