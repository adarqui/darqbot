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
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#ifndef MOD_QUEUE_H
#define MOD_QUEUE_H

#include "bot.h"

enum mod_queue_stuff
{
  MOD_QUEUE_ENQUEUE = 1,
  MOD_QUEUE_DEQUEUE,
  MOD_QUEUE_CLEAR,
  MOD_QUEUE_SIZE,
  MOD_QUEUE_LIST,
};

dlist_t *dl_mod_queue_unique;

module_t mod_queue_info;

bot_t *queue_init (dlist_t *, bot_t *);
bot_t *queue_fini (dlist_t *, bot_t *);
bot_t *queue_help (dlist_t *, bot_t *);
bot_t *queue_run (dlist_t *, bot_t *);


char *queue_change_string (bot_t *, char *, int);


void __queue_init__ (void) __attribute__ ((constructor));

#endif
