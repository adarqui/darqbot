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
#ifndef GMOD_GDAEMON_H
#define GMOD_GDAEMON_H

#include "bot.h"

typedef struct gmod_gdaemon
{
  bot_t *bot;
  dlist_t *dptr_gmod;
  bot_gmod_elm_t *gmod;

  int initialized;

/* options */
  int flags;
  char *new_name;
  char *fd_unix_path;

  dlist_t *dl_mod_names;

} gdaemon_t;

module_t gmod_gdaemon_info;

void __gdaemon_init__ (void) __attribute__ ((constructor));

bot_t *gdaemon_init (dlist_t *, bot_t *);
bot_t *gdaemon_fini (dlist_t *, bot_t *);
bot_t *gdaemon_help (dlist_t *, bot_t *);
bot_t *gdaemon_run (dlist_t *, bot_t *);
bot_t *gdaemon_control (dlist_t *, bot_t *, int, va_list);

bot_t *gdaemon_input (dlist_t *, bot_t *);
bot_t *gdaemon_output (dlist_t *, bot_t *);

bot_t *gdaemon_destroy_up (dlist_t *, bot_t *);
bot_t *gdaemon_destroy_down (dlist_t *, bot_t *);

bot_t *gdaemon_control_up (dlist_t *, bot_t *);
bot_t *gdaemon_control_down (dlist_t *, bot_t *);

bot_t *gdaemon_off (dlist_t *, bot_t *);

int gdaemon_input_client (gdaemon_t *);
char *gdaemon_change_string (bot_t *, char *, int);
char *gdaemon_process_options (gdaemon_t *, char *);
void gdaemon_process_options_parse (gdaemon_t *, char *);
void gdaemon_process_options_parse_mod_names (gdaemon_t *, char *);

bot_t *gdaemon_destroy_up_gdaemon (gdaemon_t *);
bot_t *gdaemon_destroy_down_gdaemon (gdaemon_t *);

void gdaemon_gmod_init (bot_t *, bot_gmod_elm_t *, dlist_t *);

#endif
