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

dlist_t *
files_get_listing (bot_t * bot, char *dir, int opts)
{
  dlist_t *dl, *dptr;

  dl = dptr = NULL;

  if (!dir)
    return NULL;

  files_get_listing_run (bot, dir, opts, &dl);

  return dl;
}



void
files_get_listing_run (bot_t * bot, char *dir, int opts, dlist_t ** dl)
{
  DIR *dirp;
  struct dirent *dirp_e;
  char buf[1024], *str;

  if (!dir || !dl)
    return;

  dirp = opendir (dir);
  if (!dirp)
    return;

  while (1)
    {
      dirp_e = readdir (dirp);
      if (!dirp_e)
	break;

      if (!strcmp (dirp_e->d_name, ".") || !strcmp (dirp_e->d_name, "..")
	  || dirp_e->d_type == DT_LNK)
	continue;

      if (dirp_e->d_type == DT_DIR)
	{
	  bz(buf);
	  snprintf_buf (buf, "%s%s", dir, dirp_e->d_name);
	  files_get_listing_run (bot, buf, opts, dl);
	}
      else
	{
	  str = str_unite ("%s/%s", dir, dirp_e->d_name);
	  if (str)
	    {
	      dlist_Dinsert_after (dl, str);
	    }
	}

    }

closedir(dirp);

  return;
}



void
files_destroy (bot_t * bot, dlist_t ** dl)
{

  if (!dl)
    return;

  dlist_fini (dl, free);

  return;
}
