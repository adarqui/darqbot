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
#ifndef GMOD_GPTY_H
#define GMOD_GPTY_H

#include "bot.h"

typedef struct gmod_gpty
{
  bot_t *bot;
  dlist_t *dptr_gmod;
  bot_gmod_elm_t *gmod;

  int initialized;

} gpty_t;

module_t gmod_gpty_info;

void __gpty_init__ (void) __attribute__ ((constructor));

bot_t *gpty_init (dlist_t *, bot_t *);
bot_t *gpty_fini (dlist_t *, bot_t *);
bot_t *gpty_help (dlist_t *, bot_t *);
bot_t *gpty_run (dlist_t *, bot_t *);
bot_t *gpty_control (dlist_t *, bot_t *, int, va_list);

bot_t *gpty_input (dlist_t *, bot_t *);
bot_t *gpty_output (dlist_t *, bot_t *);

bot_t *gpty_destroy_up (dlist_t *, bot_t *);
bot_t *gpty_destroy_down (dlist_t *, bot_t *);

bot_t *gpty_control_up (dlist_t *, bot_t *);
bot_t *gpty_control_down (dlist_t *, bot_t *);

bot_t *gpty_off (dlist_t *, bot_t *);

int gpty_input_client (gpty_t *);
char *gpty_change_string (bot_t *, char *, int);
char *gpty_process_options (gpty_t *, char *);
void gpty_process_options_parse (gpty_t *, char *);
void gpty_process_options_parse_null (gpty_t *, char *);

bot_t *gpty_destroy_up_gpty (gpty_t *);
bot_t *gpty_destroy_down_gpty (gpty_t *);

void gpty_gmod_init (bot_t *, bot_gmod_elm_t *, dlist_t *);

void gpty_free (void *);

#endif
