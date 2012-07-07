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
#ifndef MOD_EMOT_H
#define MOD_EMOT_H

#include "bot.h"

module_t mod_emot_info;

bot_t *emot_init(dlist_t *, bot_t *);
bot_t *emot_fini(dlist_t *, bot_t *);
bot_t *emot_help(dlist_t *, bot_t *);
bot_t *emot_run(dlist_t *, bot_t *);

typedef struct emot_toc {
	char *emot;
	char *desc;
} emot_toc_t;

typedef struct emot_info {
	int initialized;
	FILE *fp;
	emot_toc_t *toc;
	int max_toc;
} emot_info_t;

emot_info_t emot_info;

char *emot_change_string(char *, int, char *);
void emot_fill_toc(bot_t *);
char *emot_get(char *);
int emot_rand_compare(const void *, const void *);

void __emot_init__(void) __attribute__ ((constructor));

#endif
