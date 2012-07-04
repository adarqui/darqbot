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
#ifndef MOD_RSS_H
#define MOD_RSS_H

#include "bot.h"
#include <curl/curl.h>
#include <libxml/xmlreader.h>


char *rss_url;
dlist_t *rss_dl_urls;

typedef struct rss_url
{
  char *title;
  char *url;
  char *tinyurl;
} rss_url_t;

FILE *rss_fp;


module_t mod_rss_info;

bot_t *rss_init (dlist_t *, bot_t *);
bot_t *rss_fini (dlist_t *, bot_t *);
bot_t *rss_help (dlist_t *, bot_t *);
bot_t *rss_run (dlist_t *, bot_t *);


char *rss_change_string (bot_t *, char *, int, int);


size_t rss_curl_write_tinyurls_adarq (void *, size_t, size_t, void *);
size_t rss_curl_write_tinyurls_tinyurl (void *, size_t, size_t, void *);
size_t rss_curl_write_rssurls (void *, size_t, size_t, void *);
void rss_url_free (void *);
int rss_parse_xml (char *);
int rss_print_elements (xmlNode *);

void __rss_init__ (void) __attribute__ ((constructor));



enum mod_rss_method_opts
{
  RSS_OPT_METHOD_TINYURL = 0x01,
  RSS_OPT_METHOD_ADARQ = 0x02,
  RSS_OPT_UPDATE = 0x04,
  RSS_OPT_ADD = 0x08,
  RSS_OPT_DEL = 0x10,
  RSS_OPT_INFO = 0x20,
  RSS_OPT_RUN = 0x40,
  RSS_OPT_NOFORK = 0x80,
};

int rss_method;

dlist_t *dl_rss_caches;

int rss_opt_geturl (int, CURL *, rss_url_t *);
int rss_opt_geturl_adarq (CURL *, rss_url_t *);
int rss_opt_geturl_tinyurl (CURL *, rss_url_t *);

void rss_update (bot_t *, dlist_t **, int);

char *rss_op_run (bot_t *, char *, int, int);
char *rss_op_add (bot_t *, char *);
char *rss_op_del (bot_t *, char *);
char *rss_op_info (bot_t *, char *);

char *rss_ret_urldb_str (void);
char *rss_ret_url_from_key (char *);

#endif
