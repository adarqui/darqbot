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
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "bot.h"

/* irc protocol stuff, for gmod_gircd etc */
enum ircd_stuff {
IRC_NICK_ERROR = 432,
IRC_NICK_INUSE = 433,
IRC_CHANNEL_ILLEGAL = 479,
IRC_CHANNEL_USERS = 353,
IRC_CHANNEL_NOEXIST = 401,
IRC_NAMELIST_END = 366,
};

enum bot_line_commands {
BOT_LINE_PRIVMSG=1,
BOT_LINE_KICK,
BOT_LINE_BAN,
BOT_LINE_MODE,
BOT_LINE_TOPIC,
BOT_LINE_JOIN,
BOT_LINE_PART,
BOT_LINE_QUIT,
BOT_LINE_NICK,

};


typedef struct nick
{
  char *nick;
  char *username;
  char *hostname;
  char *servername;
  char *realname;
} nick_t;

typedef struct server
{
int type;
  char *host;
  int port;
  struct sockaddr_in sin;
struct sockaddr_un sun;
} server_t;

typedef struct channel
{
  char *name;
  char *key;
  int present;

dlist_t * dl_users;
char * topic;
char * modes;

} channel_t;

void nick_free(void *);

void server_free(void *);

void channel_free(void *);

void nick_add(bot_t *, char *, char *, char *, char *, char *);

dlist_t *chan_find (bot_t *, char *);

dlist_t *chan_del_user (bot_t *, dlist_t *, char *);

dlist_t *chan_add_user (bot_t *, dlist_t *, char *);

dlist_t *chan_find_user (bot_t *, dlist_t *, char *);

bot_t *chan_find_tag (char *);

#endif
