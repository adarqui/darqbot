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
#ifndef MOD_HTML_H
#define MOD_HTML_H

#include "bot.h"
#include <curl/curl.h>

module_t mod_html_info;

bot_t *html_init (dlist_t *, bot_t *);
bot_t *html_fini (dlist_t *, bot_t *);
bot_t *html_help (dlist_t *, bot_t *);
bot_t *html_run (dlist_t *, bot_t *);
void __html_init__ (void) __attribute__ ((constructor));


enum html_getopt_options{
MOD_HTML_OPT_PROXY=0x01,
MOD_HTML_OPT_REFERER=0x02,
MOD_HTML_OPT_VERBOSE=0x04,
MOD_HTML_OPT_FOLLOWLOCATION = 0x08,
MOD_HTML_OPT_POST = 0x10,
MOD_HTML_OPT_HELP= 0x20,
MOD_HTML_OPT_USERAGENT=0x40,
MOD_HTML_OPT_COOKIE=0x80,
MOD_HTML_OPT_SOCKS=0x100,
};

typedef struct html_info
{
  CURL *curl;
  CURLcode curl_res;
  char *url;
  char *cookie_file;

  struct curl_slist *cookies;

  int level;

int opt_flags;

char * opt_proxy;
char * opt_referer;
char * opt_postfields;
char * opt_useragent;
char * opt_cookie;

dlist_t * dl_text;

} html_info_t;


size_t html_curl_write (void *, size_t, size_t, void *);
size_t html_curl_read (void *, size_t, size_t, void *);



html_info_t *html_initx (bot_t *);
html_info_t *html_get (bot_t *, html_info_t *);
html_info_t *html_finix (bot_t *, html_info_t *);

char *html_change_string (bot_t *, html_info_t *, char *);

size_t html_curl_write (void *, size_t, size_t, void *);
char *html_fix_url (char *);

char * html_op_help(html_info_t *);

#endif
