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
#ifndef GMOD_GIRCPONG_H
#define GMOD_GIRCPONG_H

#include "bot.h"

typedef struct gmod_gircpong
{
  bot_t *bot;
  dlist_t *dptr_gmod;
  bot_gmod_elm_t *gmod;

  int initialized;

  int null;

} gircpong_t;

module_t gmod_gircpong_info;

void __gircpong_init__ (void) __attribute__ ((constructor));

bot_t *gircpong_init (dlist_t *, bot_t *);
bot_t *gircpong_fini (dlist_t *, bot_t *);
bot_t *gircpong_help (dlist_t *, bot_t *);
bot_t *gircpong_run (dlist_t *, bot_t *);
bot_t *gircpong_control (dlist_t *, bot_t *, int, va_list);

bot_t *gircpong_input (dlist_t *, bot_t *);
bot_t *gircpong_output (dlist_t *, bot_t *);

bot_t *gircpong_destroy_up (dlist_t *, bot_t *);
bot_t *gircpong_destroy_down (dlist_t *, bot_t *);

bot_t *gircpong_control_up (dlist_t *, bot_t *);
bot_t *gircpong_control_down (dlist_t *, bot_t *);

bot_t *gircpong_off (dlist_t *, bot_t *);

int gircpong_input_client (gircpong_t *);
char *gircpong_change_string (bot_t *, char *, int);
char *gircpong_process_options (gircpong_t *, char *);
void gircpong_process_options_parse (gircpong_t *, char *);
void gircpong_process_options_parse_null (gircpong_t *, char *);

bot_t *gircpong_destroy_up_gircpong (gircpong_t *);
bot_t *gircpong_destroy_down_gircpong (gircpong_t *);

void gircpong_gmod_init (bot_t *, bot_gmod_elm_t *, dlist_t *);

void gircpong_free (void *);

#endif
