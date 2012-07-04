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
zmodule_lock_arg (char *arg)
{

  debug (NULL, "zmodule_lock_arg: Entered\n");

  xmodule_lock (XMODULE_TYPE_MODULE, arg);
  xmodule_lock (XMODULE_TYPE_PMODULE, arg);
  xmodule_lock (XMODULE_TYPE_GMODULE, arg);
  return;
}


void
zmodule_unlock_arg (char *arg)
{

  debug (NULL, "zmodule_unlock_arg: Entered\n");

  xmodule_unlock (XMODULE_TYPE_MODULE, arg);
  xmodule_unlock (XMODULE_TYPE_PMODULE, arg);
  xmodule_unlock (XMODULE_TYPE_GMODULE, arg);
  return;
}





/* console parsing stuff */

void
console_zmodload (char *arg)
{

  xmodule_load_all (XMODULE_TYPE_MODULE);
  xmodule_load_all (XMODULE_TYPE_PMODULE);
  xmodule_load_all (XMODULE_TYPE_GMODULE);

  return;
}


void
console_zmodunload (char *arg)
{

  xmodule_unload_all (XMODULE_TYPE_MODULE);
  xmodule_unload_all (XMODULE_TYPE_PMODULE);
  xmodule_unload_all (XMODULE_TYPE_GMODULE);

  return;
}

void
console_zmodreload (char *arg)
{

  console_xmodreload (XMODULE_TYPE_MODULE, "ALL");
  console_xmodreload (XMODULE_TYPE_PMODULE, "ALL");
  console_xmodreload (XMODULE_TYPE_GMODULE, "ALL");

  return;
}




void
zmodule_iolist (void)
{
  xmodule_iolist (XMODULE_TYPE_MODULE);
  xmodule_iolist (XMODULE_TYPE_PMODULE);
  xmodule_iolist (XMODULE_TYPE_GMODULE);
  return;
}

void
zmodule_timerlist (void)
{
  xmodule_timerlist (XMODULE_TYPE_MODULE);
  xmodule_timerlist (XMODULE_TYPE_PMODULE);
  xmodule_timerlist (XMODULE_TYPE_GMODULE);
  return;
}

void
zmodule_list (void)
{
  xmodule_list (XMODULE_TYPE_MODULE);
  xmodule_list (XMODULE_TYPE_PMODULE);
  xmodule_list (XMODULE_TYPE_GMODULE);
  return;
}


void
zmodule_lock (void)
{
  module_t *mod;
  dlist_t *dl_pointer, *dlist_pointers[4], *dptr;
  int i;

  debug (NULL, "zmodule_lock: Entered\n");


  dlist_pointers[0] = gi->dl_module;
  dlist_pointers[1] = gi->dl_pmodule;
  dlist_pointers[2] = gi->dl_gmodule;
  dlist_pointers[3] = NULL;

  for (i = 0; dlist_pointers[i] != NULL; i++)
    {
      dl_pointer = dlist_pointers[i];
      dlist_fornext (dl_pointer, dptr)
      {
	mod = (module_t *) dlist_data (dptr);
	if (!mod)
	  continue;
	mod->locked = 1;
      }
    }

  return;
}


void
zmodule_unlock (void)
{
  module_t *mod;
  dlist_t *dl_pointer, *dlist_pointers[4], *dptr;
  int i;

  debug (NULL, "zmodule_unlock: Entered\n");

  dlist_pointers[0] = gi->dl_module;
  dlist_pointers[1] = gi->dl_pmodule;
  dlist_pointers[2] = gi->dl_gmodule;
  dlist_pointers[3] = NULL;

  for (i = 0; dlist_pointers[i] != NULL; i++)
    {
      dl_pointer = dlist_pointers[i];
      dlist_fornext (dl_pointer, dptr)
      {
	mod = (module_t *) dlist_data (dptr);
	if (!mod)
	  continue;
	mod->locked = 0;
      }
    }

  return;
}




void
zmodule_prune (void)
{
/* removes 'inactive modules' */

/*
dlist_fornext_retarded(gi->dl_module, dptr, dptr_tmp) {
if(!dptr) break;
module_ptr = (module_t *) dlist_data(dptr);
if(module_ptr->locked) continue;
dlist_remove_and_free(&gi->dl_module, &dptr, xmodule_free);
}

dlist_fornext_retarded(gi->dl_pmodule, dptr, dptr_tmp) {
if(!dptr) break;
module_ptr = (module_t *) dlist_data(dptr);
if(module_ptr->locked) continue;
dlist_remove_and_free(&gi->dl_pmodule, &dptr, xmodule_free);
}

dlist_fornext_retarded(gi->dl_gmodule, dptr, dptr_tmp) {
if(!dptr) break;
module_ptr = (module_t *) dlist_data(dptr);
if(module_ptr->locked) continue;
dlist_remove_and_free(&gi->dl_gmodule, &dptr, xmodule_free);
}
*/

  return;
}
