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
#ifndef MOD_INTERNALS_H
#define MOD_INTERNALS_H

#include "bot.h"

module_t mod_internals_info;

bot_t *internals_init(dlist_t *, bot_t *);
bot_t *internals_fini(dlist_t *, bot_t *);
bot_t *internals_help(dlist_t *, bot_t *);
bot_t *internals_run(dlist_t *, bot_t *);

char *internals_change_string(bot_t *, char *, int);

void __internals_init__(void) __attribute__ ((constructor));

enum internals_types {
	MOD_INTERNALS_OPT_IN_SZ = 0x01,
	MOD_INTERNALS_OPT_OUT_SZ = 0x02,
	MOD_INTERNALS_OPT_IN_DATA = 0x04,
	MOD_INTERNALS_OPT_IN_STRLEN = 0x08,
	MOD_INTERNALS_OPT_OUT_STRLEN = 0x10,
	MOD_INTERNALS_OPT_TRIG_PREFIX = 0x20,
	MOD_INTERNALS_OPT_GETPID = 0x40,
};

int internals_op_in_sz(dlist_t **, bot_t *);
int internals_op_out_sz(dlist_t **, bot_t *);
int internals_op_in_data(dlist_t **, bot_t *);
int internals_op_in_strlen(dlist_t **, bot_t *);
int internals_op_out_strlen(dlist_t **, bot_t *);
int internals_op_trig_prefix(dlist_t **, bot_t *, char *);
int internals_op_getpid(dlist_t **, bot_t *);

#endif
