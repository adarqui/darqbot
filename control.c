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

control_t *
control_init (void)
{
  control_t *control;

  debug (NULL, "control_init: Entered\n");

  control = (control_t *) calloc (1, sizeof (control_t));

  return control;
}


control_sub_t *
control_sub_init (void)
{
  control_sub_t *control_sub;

  debug (NULL, "control_sub_init: Entered\n");

  control_sub = (control_sub_t *) calloc (1, sizeof (control_sub_t));

  return control_sub;
}


dlist_t *
control_add_fdpass (control_t * control, int fd)
{
  dlist_t *dptr = NULL;
  control_sub_t *control_sub;

  debug (NULL, "control_add_fdpass: Entered\n");

  if (!control)
    return NULL;

  control_sub = control_sub_init ();
  if (!control_sub)
    return NULL;

  control_sub->type = CONTROL_SUB_FDPASS;
  control_sub->val = fd;

  dptr = dlist_Dinsert_after (&control->dl_subs, control_sub);
  return dptr;
}


dlist_t *
control_bot_add (bot_t * bot, control_t * control)
{
  dlist_t *dptr = NULL;

  debug (NULL, "control_bot_add: Entered\n");

  if (!bot || !control)
    return NULL;
  dptr = dlist_Dinsert_after (&bot->dl_control, control);
  return dptr;
}

void
control_bot_del (bot_t * bot, dlist_t * dptr_control)
{
  dlist_t *dptr, *dptr_tmp;

  debug (NULL, "control_bot_del: Entered\n");

  if (!bot || !dptr_control)
    return;

  dlist_fornext_retarded (bot->dl_control, dptr, dptr_tmp)
  {
    if (!dptr)
      break;

    if (dptr == dptr_control)
      {
	dlist_remove_and_free (&bot->dl_control, &dptr, control_fini);
	break;
      }
  }

  return;
}


int
control_get_fdpass (dlist_t ** dl_control)
{
  control_t *control;
  control_sub_t *control_sub;
  dlist_t *dptr_control, *dptr_control_sub, *dptr_tmp_1, *dptr_tmp_2;
  int fd;

  debug (NULL, "control_get_fdpass: Entered\n");

  if (!dl_control)
    return -1;

  dlist_fornext_retarded (*dl_control, dptr_control, dptr_tmp_1)
  {
    if (!dptr_control)
      break;
    control = (control_t *) dlist_data (dptr_control);
    dlist_fornext_retarded (control->dl_subs, dptr_control_sub, dptr_tmp_2)
    {
      if (!dptr_control_sub)
	break;
      control_sub = (control_sub_t *) dlist_data (dptr_control_sub);
      if (control_sub->type == CONTROL_SUB_FDPASS)
	{
	  fd = control_sub->val;
	  debug (NULL, "control_get_fdpass_find; FOUND %i\n",
		     control_sub->val);
	  dlist_remove_and_free (&control->dl_subs, &dptr_control_sub,
				 control_sub_fini);
	  return fd;
	}
    }
  }

  return -1;
}



int
control_get_fdpass_find (dlist_t ** dl_control)
{
  control_t *control;
  control_sub_t *control_sub;
  dlist_t *dptr_control, *dptr_control_sub, *dptr_tmp_1, *dptr_tmp_2;
  int fd;

  debug (NULL, "control_get_fdpass: Entered\n");

  if (!dl_control)
    return -1;

  dlist_fornext_retarded (*dl_control, dptr_control, dptr_tmp_1)
  {
    if (!dptr_control)
      break;
    control = (control_t *) dlist_data (dptr_control);
    dlist_fornext_retarded (control->dl_subs, dptr_control_sub, dptr_tmp_2)
    {
      if (!dptr_control_sub)
	break;
      control_sub = (control_sub_t *) dlist_data (dptr_control_sub);
      if (control_sub->type == CONTROL_SUB_FDPASS)
	{
	  debug (NULL, "control_get_fdpass_find; FOUND %i\n",
		     control_sub->val);
	  fd = control_sub->val;
	  return fd;
	}
    }
  }

  return -1;
}

void
control_fini (void *arg)
{
  control_t *control = (control_t *) arg;

  debug (NULL, "control_fini: Entered\n");

  if (!control)
    return;

  dlist_fini (&control->dl_subs, control_sub_fini);

  free (control);

  return;
}


void
control_sub_fini (void *arg)
{
  control_sub_t *control_sub = (control_sub_t *) arg;

  debug (NULL, "control_sub_fini: Entered\n");

  if (!control_sub)
    return;

/* ... */

  free (control_sub);

  return;
}
