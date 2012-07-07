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
#ifndef MOD_SHAKE1_H
#define MOD_SHAKE1_H

#include "bot.h"

module_t mod_shake1_info;

bot_t *shake1_init(dlist_t *, bot_t *);
bot_t *shake1_fini(dlist_t *, bot_t *);
bot_t *shake1_help(dlist_t *, bot_t *);
bot_t *shake1_run(dlist_t *, bot_t *);

typedef struct shake1_info {
	int initialized;

	FILE *fp_1;
	int fp_1_lines;
	off_t *fp_1_offsets;

	FILE *fp_2;
	int fp_2_lines;
	off_t *fp_2_offsets;

	FILE *fp_3;
	int fp_3_lines;
	off_t *fp_3_offsets;

} shake1_info_t;

shake1_info_t shake1_info;

char *shake1_change_string(char *, int);
int shake1_fill_toc_info(bot_t *, FILE *, int *, off_t **);
void shake1_print_info(bot_t *);
void shake1_fill_toc(bot_t *);

void __shake1_init__(void) __attribute__ ((constructor));

#endif
