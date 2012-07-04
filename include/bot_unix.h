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
#ifndef BOT_UNIX_H
#define BOT_UNIX_H

#include "bot.h"

#define BOT_UNIX_SOCKPATH "/tmp/unix.sock"

enum bot_unix_ops {
BOT_UNIX_OP_FDPASS=1,
BOT_UNIX_OP_COMM,
BOT_UNIX_OP_TAG,
BOT_UNIX_OP_CLOSE,
};

typedef struct bot_unix_node {
char tag[32];
int op;
struct event ev;
int ev_fd;
int passed_fd;
time_t t;
}
bot_unix_node_t;

dlist_t * dl_bot_unix;


int bot_unix_fd_send(bot_t *, int,bot_gmod_elm_t*);

int bot_unix_init(void);

int bot_unix_fini(void);

void bot_evhook_unix_accept (int , short , void *);

void bot_evhook_unix_read(int, short, void *);

void bot_unix_node_destroy(void *);

#endif
