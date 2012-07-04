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
#ifndef MOD_TINY_H
#define MOD_TINY_H

#include "bot.h"
#include <curl/curl.h>

char *tiny_url;

module_t mod_tiny_info;

bot_t *tiny_init (dlist_t *, bot_t *);
bot_t *tiny_fini (dlist_t *, bot_t *);
bot_t *tiny_help (dlist_t *, bot_t *);
bot_t *tiny_run (dlist_t *, bot_t *);


char *tiny_change_string (bot_t *, char *, int);


size_t tiny_curl_write (void *, size_t, size_t, void *);


void __tiny_init__ (void) __attribute__ ((constructor));

#endif
