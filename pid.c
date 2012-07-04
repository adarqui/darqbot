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
/* these routines are use to track & kill pids spawned by various bot objects */

#include "bot.h"

xpid_t *
xpid_add (bot_t * bot, pid_t pid)
{
  xpid_t *bp = NULL;

debug(NULL, "xpid_add: Entered: pid=%i\n",pid);

  if (!bot || pid <= 1)
    return NULL;

  bp = (xpid_t *) calloc (1, sizeof (xpid_t));
  if (!bp)
    return NULL;


  bp->tag = strdup (bot->tag);
  bp->pid = pid;

  dlist_Dinsert_after (&gi->dl_pid, bp);

  return bp;
}


void
xpid_destroy (bot_t * bot, int signum)
{
  dlist_t *dptr_bp = NULL, *dptr_tmp;
  xpid_t *bp = NULL;


debug(NULL, "xpid_destroy: Entered\n");

  if (!bot || signum < 0)
    return;

  dlist_fornext_retarded (gi->dl_pid, dptr_bp, dptr_tmp)
  {
    if (!dptr_bp)
      break;
    bp = (xpid_t *) dlist_data (dptr_bp);
    if (!bp)
      continue;

    if (!strcasecmp (bp->tag, bot->tag))
      {
	if (bp->pid > 1)
	  kill (bp->pid, signum);

	dlist_remove_and_free (&gi->dl_pid, &dptr_bp, xpid_free);
      }
  }

  return;
}



void
xpid_del (bot_t * bot, pid_t pid)
{
  xpid_t *bp = NULL;
  dlist_t *dptr_bp, *dptr_tmp;

debug(NULL, "xpid_del: Entered: pid=%i\n", pid);

  if (pid <= 1)
    return;

  dlist_fornext_retarded (gi->dl_pid, dptr_bp, dptr_tmp)
  {
    if (!dptr_bp)
      break;
    bp = (xpid_t *) dlist_data (dptr_bp);
    if (!bp)
      continue;

    if (bp->pid == pid)
      {
	dlist_remove_and_free (&gi->dl_pid, &dptr_bp, xpid_free);
	return;
      }

  }

  return;
}




void
xpid_free (void *arg)
{
  xpid_t *bp = (xpid_t *) arg;

debug(NULL, "xpid_free: Entered\n");

  if (!bp)
    return;

  if (bp->tag)
    free (bp->tag);

  free (bp);

  return;
}




void
xpid_console_print (bot_t * bot, char *tag)
{
  dlist_t *dptr = NULL;
  xpid_t *bp = NULL;

debug(NULL, "xpid_console_print: Entered: tag=%s\n", tag);


  if (!bot)
    return;

  bot_dl_print (bot, "dl_pid: size=%i\n", dlist_size (gi->dl_pid));

  dlist_fornext (gi->dl_pid, dptr)
  {
    bp = (xpid_t *) dlist_data (dptr);
    if (!bp)
      continue;

    if (tag)
      {
	if (strcasecmp (bp->tag, tag))
	  continue;
      }

    bot_dl_print (bot, "\ttag=%s : pid=%i\n", bp->tag, bp->pid);
  }

  return;
}




void
xpid_waitpid (void)
{
  pid_t pid;
  int status;

  pid = waitpid (-1, &status, WNOHANG);

  debug (NULL, "xpid_waitpid: pid=%i\n", pid);

  if (pid > 1)
    {
      xpid_del (NULL, pid);
    }

  return;
}
