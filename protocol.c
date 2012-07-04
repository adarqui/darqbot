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
nick_add (bot_t * bot, char *nick, char *username, char *hostname,
	  char *servername, char *realname)
{
  nick_t *nick_ptr;

  debug (NULL, "nick_add: Entered\n");

  if (!nick || !username || !hostname || !servername || !realname || !bot)
    return;

  nick_ptr = (nick_t *) calloc (1, sizeof (nick_t));
  nick_ptr->nick = strdup (nick);
  nick_ptr->username = strdup (username);
  nick_ptr->hostname = strdup (hostname);
  nick_ptr->servername = strdup (servername);
  nick_ptr->realname = strdup (realname);

  return;
}

void
nick_free (void *arg)
{
  nick_t *nick_ptr;

  if (!arg)
    return;

  nick_ptr = (nick_t *) arg;
  if (nick_ptr->nick)
    free (nick_ptr->nick);

  if (nick_ptr->username)
    free (nick_ptr->username);

  if (nick_ptr->hostname)
    free (nick_ptr->hostname);

  if (nick_ptr->servername)
    free (nick_ptr->servername);

  if (nick_ptr->realname)
    free (nick_ptr->realname);

  memset (nick_ptr, 0, sizeof (nick_t));

  free (nick_ptr);

  return;
}




void
server_free (void *arg)
{
  server_t *server_ptr;

  if (!arg)
    return;

  server_ptr = (server_t *) arg;

  if (server_ptr->host)
    free (server_ptr->host);

  memset (server_ptr, 0, sizeof (server_t));

  free (server_ptr);

  return;
}


void
channel_free (void *arg)
{
  channel_t *channel_ptr;

  if (!arg)
    return;

  channel_ptr = (channel_t *) arg;

  if (channel_ptr->name)
    free (channel_ptr->name);

  if (channel_ptr->key)
    free (channel_ptr->key);

  if (channel_ptr->topic)
    free (channel_ptr->topic);

  if (channel_ptr->modes)
    free (channel_ptr->modes);

  dlist_fini (&channel_ptr->dl_users, free);

  memset (channel_ptr, 0, sizeof (channel_t));

  free (channel_ptr);

  return;
}








dlist_t *
chan_find (bot_t * bot, char *name)
{
  dlist_t *dptr;
  int found = 0;

  if (!bot || !name)
    return NULL;

  if (found)
    return dptr;

  return NULL;
}








bot_t *
chan_find_tag (char *tag)
{
  dlist_t *dptr;
  bot_t *bot = NULL;

  if (!tag)
    return NULL;

  dlist_fornext (gi->bots, dptr)
  {
    bot = (bot_t *) dlist_data (dptr);
    if (!bot)
      continue;

    if (!strcasecmp (bot->tag, tag))
      {
	return bot;
      }

  }

  return NULL;
}





dlist_t *
chan_del_user (bot_t * bot, dlist_t * dl, char *name)
{
  dlist_t *dptr;
  char *user;

  dptr = NULL;

  if (!bot || !dl || !name)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    user = (char *) dlist_data (dptr);
    if (!strcasecmp (user, name))
      {
	dptr = dlist_remove (&dl, dptr);
	free (user);
	free (dptr);
	return dl;
      }

  }

  return NULL;
}

dlist_t *
chan_find_user (bot_t * bot, dlist_t * dl, char *name)
{
  dlist_t *dptr = NULL;
  char *user;

  if (!dl || !bot || !name)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    user = (char *) dlist_data (dptr);
    if (!user)
      continue;
    if (!strcasecmp (user, name))
      return dptr;
  }

  return dptr;
}







dlist_t *
chan_add_user (bot_t * bot, dlist_t * dl, char *name)
{

  dlist_t *dptr;

  dptr = NULL;

  if (!bot || !dl || !name)
    return NULL;

  dptr = dlist_Dinsert_after (&dl, name);
  return dptr;

}
