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
#ifndef FD_H
#define FD_H

#include "bot.h"

typedef struct fd_link {

int type;
  int fd;
  struct event ev;
  struct sockaddr_in sin;
struct sockaddr_un sun;

} fd_link_t;



fd_link_t * fd_link_add(dlist_t **, int, struct event, struct sockaddr_in *, struct sockaddr_un *);

fd_link_t * fd_link_find(dlist_t **, int) ;

int fd_link_del(dlist_t **, int, void (*cleanup)(fd_link_t *)) ;

void fd_link_destroy(dlist_t ** , void (*cleanup)(fd_link_t *)) ;

void fd_link_cleanup(fd_link_t * ) ;

void fd_link_set_cur(dlist_t **, fd_link_t *, int);

void fd_link_print(fd_link_t *);

char  * fd_link_get_latest_host(bot_t *  , char * ,char *);

#endif
