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
#ifndef MOD_VAR_H
#define MOD_VAR_H

#include "bot.h"

module_t mod_var_info;

bot_t *var_init(dlist_t *, bot_t *);
bot_t *var_fini(dlist_t *, bot_t *);
bot_t *var_help(dlist_t *, bot_t *);
bot_t *var_run(dlist_t *, bot_t *);

char *var_usleep_fn(bot_t *, char *);
char *var_multi_fn(bot_t *, char *);
char *var_bufsz_fn(bot_t *, char *);
char *var_maxflood_fn(bot_t *, char *);
char *var_nesting_fn(bot_t *, char *);
char *var_nesting_cur_fn(bot_t *, char *);
char *var_allowpm_fn(bot_t *, char *);
char *var_sigprotect_fn(bot_t *, char *);
char *var_timer_sleep_fn(bot_t *, char *);
char *var_timer_sort_fn(bot_t *, char *);

void __var_init__(void) __attribute__ ((constructor));

#endif
