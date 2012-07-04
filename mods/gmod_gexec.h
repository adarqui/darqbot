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
#ifndef GMOD_GEXEC_H
#define GMOD_GEXEC_H

#include "bot.h"

typedef struct gmod_gexec
{
  bot_t *bot;
  dlist_t *dptr_gmod;
  bot_gmod_elm_t *gmod;

  int initialized;

char * bin;
int pipe[2];
int pipe_b[2];

/* for libevent */
  fd_link_t link;

pid_t pid;

} gexec_t;

module_t gmod_gexec_info;

void __gexec_init__ (void) __attribute__ ((constructor));

bot_t *gexec_init (dlist_t *, bot_t *);
bot_t *gexec_fini (dlist_t *, bot_t *);
bot_t *gexec_help (dlist_t *, bot_t *);
bot_t *gexec_run (dlist_t *, bot_t *);
bot_t *gexec_control (dlist_t *, bot_t *, int, va_list);

bot_t *gexec_input (dlist_t *, bot_t *);
bot_t *gexec_output (dlist_t *, bot_t *);

bot_t *gexec_destroy_up (dlist_t *, bot_t *);
bot_t *gexec_destroy_down (dlist_t *, bot_t *);

bot_t *gexec_control_up (dlist_t *, bot_t *);
bot_t *gexec_control_down (dlist_t *, bot_t *);

bot_t *gexec_off (dlist_t *, bot_t *);

int gexec_input_client (gexec_t *);
char *gexec_change_string (bot_t *, char *, int);
char *gexec_process_options (gexec_t *, char *);
void gexec_process_options_parse (gexec_t *, char *);
void gexec_process_options_parse_bin (gexec_t *, char *);

bot_t *gexec_destroy_up_gexec (gexec_t *);
bot_t *gexec_destroy_down_gexec (gexec_t *);

void gexec_gmod_init (bot_t *, bot_gmod_elm_t *, dlist_t *);

void gexec_free (void *);
int gexec_exec(gexec_t *);

void gexec_evhook_link (int, short, void *) ;
int gexec_set_evhooks (gexec_t * );
int gexec_unset_evhooks(gexec_t *);
void gexec_free(void *);


#endif
