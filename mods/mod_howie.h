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
#ifndef MOD_HOWIE_H
#define MOD_HOWIE_H

#include "bot.h"
#include <curl/curl.h>

char *howie_url;

module_t mod_howie_info;

bot_t *howie_init (dlist_t *, bot_t *);
bot_t *howie_fini (dlist_t *, bot_t *);
bot_t *howie_help (dlist_t *, bot_t *);
bot_t *howie_run (dlist_t *, bot_t *);

void howie_fill_info (bot_t *);
int howie_run_on (bot_t *);
int howie_run_off (bot_t *);


typedef struct howie_info
{
  int initialized;

  pid_t ison;

  FILE *fp;

  int argc;
  char **argv;
  char *dir;

  int pipefd[2];
  int pipefd_b[2];

  int should_on;

}
howie_info_t;

howie_info_t howie_info;


char *howie_change_string (bot_t *, char *, int);


size_t howie_curl_write (void *, size_t, size_t, void *);


void __howie_init__ (void) __attribute__ ((constructor));

#endif
