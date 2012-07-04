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
#ifndef PMOD_PARSE2_H
#define PMOD_PARSE2_H

#include "bot.h"

int parse2_nesting;

enum text_list_types
{
  TEXT_LIST_ROOT = 1,
  TEXT_LIST_DATA = 0,
  TEXT_LIST_DEACTIVATED = 2,
};

typedef struct parse2_text_list
{
  int type;
  char *string;
  char *string_new;
  char *result;
  dlist_t *children;
} text_list_t;

dlist_t * dl_pmod_parse2_unique;

typedef struct parse2_data {
unsigned char initialized;
dlist_t * allow;
dlist_t * deny;
} parse2_data_t;

/*
dlist_t *dl_parse2_allow;
dlist_t *dl_parse2_deny;
*/

void parse2_build_text_list (dlist_t ** dl, char *input, int len, int type);
void parse2_build_text_list_print (dlist_t * dl);
void parse2_text_list_free (void *tl_arg);
void parse2_text_list_destroy (dlist_t ** dl_text_list);
void parse2_build_text_list_run (dlist_t * dl_text, dlist_t * dl_node,
				 bot_t * bot, void *arg_info);
int parse2_build_text_list_replace_last (dlist_t * dl_text_list,
					 text_list_t * tl);

void parse2_process_options (dlist_t *, bot_t *, char *, parse2_data_t *);
void parse2_process_options_parse_allow (void *, bot_t *, char *, parse2_data_t *);
void parse2_process_options_parse_deny (void *, bot_t *, char *, parse2_data_t *);
void parse2_process_options_parse (void *, bot_t *, char *, parse2_data_t *);

void parse2_allow_add (parse2_data_t *, char *);
void parse2_allow_clear (parse2_data_t *);
int parse2_allow_find (parse2_data_t *, char *);

void parse2_deny_add (parse2_data_t *, char *);
void parse2_deny_clear (parse2_data_t *);
int parse2_deny_find (parse2_data_t *,char *);

module_t pmod_parse2_info;

bot_t *parse2_init (dlist_t *, bot_t *);
bot_t *parse2_fini (dlist_t *, bot_t *);
bot_t *parse2_help (dlist_t *, bot_t *);
bot_t *parse2_run (dlist_t *, bot_t *);

parse2_data_t * parse2_data_init(void);
void parse2_data_fini(void *);

int parse2_handle_text (dlist_t *, bot_t *, void *);

void __parse2_init__ (void) __attribute__ ((constructor));

#endif
