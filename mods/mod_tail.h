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
#ifndef MOD_TAIL_H
#define MOD_TAIL_H

#include "bot.h"

enum mod_tail_stuff
{
  MOD_TAIL_ADD = 1,
  MOD_TAIL_DEL,
  MOD_TAIL_CLEAR,
  MOD_TAIL_LIST,
  MOD_TAIL_RUN
};

dlist_t *dl_mod_tail_unique;

module_t mod_tail_info;

bot_t *tail_init (dlist_t *, bot_t *);
bot_t *tail_fini (dlist_t *, bot_t *);
bot_t *tail_help (dlist_t *, bot_t *);
bot_t *tail_run (dlist_t *, bot_t *);


char *tail_change_string (bot_t *, char *, int);


void __tail_init__ (void) __attribute__ ((constructor));


typedef struct tail
{
  int active;
  char *filename;
  off_t off;
  struct stat st;
} tail_t;

char *tail_op_clear (dlist_t **);
char *tail_op_run (dlist_t **);
char *tail_op_list (dlist_t **);
char *tail_op_add (dlist_t **, char *);
char *tail_op_del (dlist_t **, char *);

tail_t *tail_init_node (void);
tail_t *tail_find_node (dlist_t **, char *);
int tail_open_and_read (tail_t *, char *, int);
void tail_free (void *);

#endif
