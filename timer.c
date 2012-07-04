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
#include <bot.h>

void
timer_handler (int arg)
{

  event_loopbreak ();
  gi->timer.toggle = 1;

  return;
}

void
timer_handler_new (int arg)
{
  dlist_t *dptr;
  module_t *module;
  pid_t pid;

  debug (NULL, "timer_handler: Entered\n");

  pid = getpid ();

  if (pid != gi->pid)
    return;

  time (&gi->timer.last_timer);

  timer_set ();

  dlist_fornext (gi->dl_mod_timers, dptr)
  {

    module = (module_t *) dlist_data (dptr);

    if (!module)
      continue;
    if (module->timer)
      {
	module->timer (NULL, NULL);
      }

  }


  dlist_fornext (gi->dl_gmod_gtimers, dptr)
  {

    module = (module_t *) dlist_data (dptr);

    if (!module)
      continue;
    if (module->active)
      {
	if (module->timer)
	  {
	    module->timer (NULL, NULL);
	  }
      }
  }

  if ((gi->timer.last_timer - gi->timer.last_sort) > gi->var_timer_sort)
    {
/* do sorting */
      time (&gi->timer.last_sort);
      debug (NULL, "DOING SORT, %i %i\n",
	     (gi->timer.last_timer - gi->timer.last_sort) >
	     gi->var_timer_sort, gi->var_timer_sort);
      timer_sort_modules ();
    }

  return;
}


void
timer_set (void)
{

  if (gi->var_timer_sleep <= 0)
    return;

  signal (SIGALRM, timer_handler);
  alarm (gi->var_timer_sleep);

  return;
}


void
timer_unset (void)
{
  signal (SIGALRM, bot_sigend_handler);
  alarm (0);
  return;
}



void
timer_shouldwerun (void)
{
  if (gi->timer.toggle)
    {
      gi->timer.toggle = 0;
      timer_handler_new (SIGALRM);
    }

}




void
timer_sort_modules (void)
{


/*
  module_update_stats ();
  pmodule_update_stats ();
  gmodule_update_stats ();

  module_sort ();
  pmodule_sort ();
  gmodule_sort ();
*/

  return;
}
