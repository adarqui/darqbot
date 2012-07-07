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
#ifndef GMOD_GSSL_H
#define GMOD_GSSL_H

#include "bot.h"

typedef struct gmod_gssl {
	bot_t *bot;
	dlist_t *dptr_gmod;
	bot_gmod_elm_t *gmod;

	int initialized;

	BIO *ibio;
	BIO *obio;
	SSL_CTX *ctx;
	SSL *ssl;

} gssl_t;

module_t gmod_gssl_info;

void __gssl_init__(void) __attribute__ ((constructor));

bot_t *gssl_init(dlist_t *, bot_t *);
bot_t *gssl_fini(dlist_t *, bot_t *);
bot_t *gssl_help(dlist_t *, bot_t *);
bot_t *gssl_run(dlist_t *, bot_t *);
bot_t *gssl_control(dlist_t *, bot_t *, int, va_list);

bot_t *gssl_input(dlist_t *, bot_t *);
bot_t *gssl_output(dlist_t *, bot_t *);

bot_t *gssl_destroy_up(dlist_t *, bot_t *);
bot_t *gssl_destroy_down(dlist_t *, bot_t *);

bot_t *gssl_control_up(dlist_t *, bot_t *);
bot_t *gssl_control_down(dlist_t *, bot_t *);

bot_t *gssl_off(dlist_t *, bot_t *);

int gssl_input_client(gssl_t *);
char *gssl_change_string(bot_t *, char *, int);
char *gssl_process_options(gssl_t *, char *);
void gssl_process_options_parse(gssl_t *, char *);
void gssl_process_options_parse_null(gssl_t *, char *);

bot_t *gssl_destroy_up_gssl(gssl_t *);
bot_t *gssl_destroy_down_gssl(gssl_t *);

void gssl_gmod_init(bot_t *, bot_gmod_elm_t *, dlist_t *);

void gssl_free(void *);
int gssl_handshake(gssl_t *);
void gssl_check_status(gssl_t *);

#endif
